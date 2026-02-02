#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "SpartaGameMode.generated.h"

class ASpartaGameState;

UCLASS()
class SPARTACPP_API ASpartaGameMode : public AGameMode
{
	GENERATED_BODY()


public:
	ASpartaGameMode();
	
	virtual void BeginPlay() override;

	void StartNextWave();
	void EndWave();

	void SpawnItems(int32 Count);

protected:
	UPROPERTY()
	TObjectPtr<ASpartaGameState> SpartaGameState;
};
