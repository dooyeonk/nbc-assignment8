#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SpartaGameState.generated.h"

struct FWaveData;

DECLARE_MULTICAST_DELEGATE(FOnAllCoinCollected);
DECLARE_MULTICAST_DELEGATE(FOnWaveTimeUp);

UCLASS()
class SPARTACPP_API ASpartaGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	ASpartaGameState();
	virtual void BeginPlay() override;

	FOnAllCoinCollected OnAllCoinCollected;
	FOnWaveTimeUp OnWaveTimeUpDelegate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score")
	int32 Score;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 SpawnedCoinCount;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 CollectedCoinCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 CurrentLevelIndex;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 MaxLevels;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	TArray<FName> LevelMapNames;
	
	FTimerHandle HUDUpdateTimerHandle;

	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const;

	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);
	
	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameOver();
	
	void StartLevel();
	void EndLevel();
	void OnCoinCollected();
	void UpdateHUD();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Wave")
	TObjectPtr<UDataTable> WaveDataTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 CurrentWaveIndex;

	void OnWaveTimeUp();

	FTimerHandle WaveTimerHandle;
	FWaveData* GetCurrentWaveData();
};
