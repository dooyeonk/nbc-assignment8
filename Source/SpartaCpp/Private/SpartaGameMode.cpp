#include "SpartaGameMode.h"

#include "CoinItem.h"
#include "SpartaCharacter.h"
#include "SpartaGameInstance.h"
#include "SpartaGameState.h"
#include "SpartaHUD.h"
#include "SpartaPlayerController.h"
#include "SpawnVolume.h"
#include "WaveData.h"
#include "Kismet/GameplayStatics.h"

ASpartaGameMode::ASpartaGameMode()
{
	DefaultPawnClass = ASpartaCharacter::StaticClass();
	PlayerControllerClass = ASpartaPlayerController::StaticClass();
	GameStateClass = ASpartaGameState::StaticClass();
	HUDClass = ASpartaHUD::StaticClass();

	SpartaGameState = nullptr;
}

void ASpartaGameMode::BeginPlay()
{
	Super::BeginPlay();

	SpartaGameState = GetGameState<ASpartaGameState>();
	if (SpartaGameState)
	{
		SpartaGameState->OnAllCoinCollected.AddUObject(this, &ASpartaGameMode::EndWave);
		SpartaGameState->OnWaveTimeUpDelegate.AddUObject(this, &ASpartaGameMode::EndWave);

		SpartaGameState->StartLevel();
	}

	StartNextWave();
}

void ASpartaGameMode::StartNextWave()
{
	if (!SpartaGameState) return;

	FWaveData* CurrentWave = SpartaGameState->GetCurrentWaveData();
	if (CurrentWave)
	{
		SpartaGameState->SpawnedCoinCount = 0;
		SpartaGameState->CollectedCoinCount = 0;

		SpawnItems(CurrentWave->ItemSpawnCount);

		GetWorldTimerManager().SetTimer(
			SpartaGameState->WaveTimerHandle,
			SpartaGameState.Get(),
			&ASpartaGameState::OnWaveTimeUp,
			CurrentWave->WaveDuration,
			false
		);

		UE_LOG(LogTemp, Warning, TEXT("Wave %d Started"), SpartaGameState->CurrentWaveIndex + 1);
	}
}

void ASpartaGameMode::EndWave()
{
	if (!SpartaGameState) return;

	GetWorldTimerManager().ClearTimer(SpartaGameState->WaveTimerHandle);

	SpartaGameState->CurrentWaveIndex++;

	if (SpartaGameState->GetCurrentWaveData())
	{
		StartNextWave();
	}
	else
	{
		SpartaGameState->EndLevel();
	}
}

void ASpartaGameMode::SpawnItems(int32 Count)
{
	if (!SpartaGameState) return;

	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	if (FoundVolumes.Num() > 0)
	{
		ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
		for (int32 i = 0; i < Count; i++)
		{
			AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
			if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
			{
				SpartaGameState->SpawnedCoinCount++;
			}
		}
	}
}
