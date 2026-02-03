#include "SpartaGameState.h"

#include "SpartaGameInstance.h"
#include "SpartaHUD.h"
#include "SpartaPlayerController.h"
#include "WaveData.h"
#include "Kismet/GameplayStatics.h"

ASpartaGameState::ASpartaGameState()
{
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	CurrentLevelIndex = 0;
	CurrentWaveIndex = 0;
	MaxLevels = 3;

	WaveDataTable = nullptr;
}

void ASpartaGameState::BeginPlay()
{
	Super::BeginPlay();

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance))
		{
			CurrentLevelIndex = SpartaGameInstance->CurrentLevelIndex;
		}
	}

	GetWorldTimerManager().SetTimer(HUDUpdateTimerHandle, this, &ASpartaGameState::UpdateHUD, 0.1f, true);
}

int32 ASpartaGameState::GetScore() const
{
	return Score;
}

void ASpartaGameState::AddScore(int32 Amount)
{
	Score += Amount;
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance))
		{
			SpartaGameInstance->AddScore(Amount);
		}
	}
}

void ASpartaGameState::OnGameOver()
{
	if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(
		GetWorld()->GetFirstPlayerController()))
	{
		SpartaPlayerController->SetPause(true);
		if (ASpartaHUD* SpartaHUD = SpartaPlayerController->GetHUD<ASpartaHUD>())
		{
			SpartaHUD->ShowMainMenu(true);
		}
	}
}

void ASpartaGameState::StartLevel()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			if (ASpartaHUD* SpartaHUD = SpartaPlayerController->GetHUD<ASpartaHUD>())
			{
				SpartaHUD->ShowGameHUD();
			}
		}
	}

	CurrentWaveIndex = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
}

void ASpartaGameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(WaveTimerHandle);
	GetWorldTimerManager().ClearTimer(HUDUpdateTimerHandle);

	CurrentLevelIndex++;

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance))
		{
			SpartaGameInstance->CurrentLevelIndex = CurrentLevelIndex;
		}
	}

	if (CurrentLevelIndex >= MaxLevels)
	{
		OnGameOver();
		return;
	}

	if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
	}
	else
	{
		OnGameOver();
	}
}

void ASpartaGameState::OnCoinCollected()
{
	CollectedCoinCount++;

	if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
	{
		if (OnAllCoinCollected.IsBound())
		{
			OnAllCoinCollected.Broadcast();
		}
	}
}

void ASpartaGameState::UpdateHUD()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController) return;

	if (ASpartaHUD* SpartaHUD = PlayerController->GetHUD<ASpartaHUD>())
	{
		float RemainingTime = GetWorldTimerManager().GetTimerRemaining(WaveTimerHandle);
		if (RemainingTime < 0.0f) RemainingTime = 0.0f;

		SpartaHUD->UpdateHUD(RemainingTime, Score, CurrentLevelIndex, CurrentWaveIndex);
	}
}

void ASpartaGameState::OnWaveTimeUp()
{
	if (OnWaveTimeUpDelegate.IsBound())
	{
		OnWaveTimeUpDelegate.Broadcast();
	}
}

FWaveData* ASpartaGameState::GetCurrentWaveData()
{
	if (!WaveDataTable) return nullptr;

	TArray<FName> RowNames = WaveDataTable->GetRowNames();
	if (RowNames.IsValidIndex(CurrentWaveIndex))
	{
		static const FString ContextString(TEXT("WaveDataLookup"));
		return WaveDataTable->FindRow<FWaveData>(RowNames[CurrentWaveIndex], ContextString);
	}
	return nullptr;
}
