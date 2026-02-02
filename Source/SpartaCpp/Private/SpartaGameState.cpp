#include "SpartaGameState.h"

#include "CoinItem.h"
#include "SpartaGameInstance.h"
#include "SpartaHUD.h"
#include "SpartaPlayerController.h"
#include "SpawnVolume.h"
#include "Kismet/GameplayStatics.h"

ASpartaGameState::ASpartaGameState()
{
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	CurrentLevelIndex = 0;
	LevelDuration = 30.0f;
	MaxLevels = 3;
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

	StartLevel();
	GetWorldTimerManager().SetTimer(HUDUpdateTimerHandle, this, &ASpartaGameState::UpdateHUD, 0.1f, true);
}

int32 ASpartaGameState::GetScore() const
{
	return Score;
}

void ASpartaGameState::AddScore(int32 Amount)
{
	Score += Amount;
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
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;

	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	// TODO: dooyeonk, Wave System
	const int32 ItemToSpawn = 40;

	for (int32 i = 0; i < ItemToSpawn; i++)
	{
		if (FoundVolumes.Num() > 0)
		{
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
			if (SpawnVolume)
			{
				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
				if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
				{
					SpawnedCoinCount++;
				}
			}
		}
	}

	GetWorldTimerManager().SetTimer(LevelTimerHandle, this, &ASpartaGameState::OnLevelTimeUp, LevelDuration, false);
}

void ASpartaGameState::OnLevelTimeUp()
{
	EndLevel();
}

void ASpartaGameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);
	CurrentLevelIndex++;

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance))
		{
			SpartaGameInstance->AddScore(Score);
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
		EndLevel();
	}
}

void ASpartaGameState::UpdateHUD()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController) return;

	if (ASpartaHUD* SpartaHUD = PlayerController->GetHUD<ASpartaHUD>())
	{
		float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
		SpartaHUD->UpdateHUD(RemainingTime, Score, CurrentLevelIndex);
	}
}
