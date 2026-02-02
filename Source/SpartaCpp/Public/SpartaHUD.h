#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SpartaHUD.generated.h"

UCLASS()
class SPARTACPP_API ASpartaHUD : public AHUD
{
	GENERATED_BODY()

public:
	ASpartaHUD();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> HUDWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> CurrentWidget;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowMainMenu(bool bIsRestart = false);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowGameHUD();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ClearHUD();

	void UpdateHUD(float RemainingTime, int32 Score, int32 Level);
};
