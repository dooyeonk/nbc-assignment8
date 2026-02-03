#include "SpartaHUD.h"

#include "SpartaGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

ASpartaHUD::ASpartaHUD() : MainMenuWidgetClass(nullptr), HUDWidgetClass(nullptr), CurrentWidget(nullptr)
{
}

void ASpartaHUD::ShowMainMenu(bool bIsRestart)
{
	ClearHUD();

	if (MainMenuWidgetClass)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), MainMenuWidgetClass);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();

			if (APlayerController* PlayerController = GetOwningPlayerController())
			{
				PlayerController->SetInputMode(FInputModeUIOnly());
				PlayerController->bShowMouseCursor = true;
			}

			if (UTextBlock* ButtonText = Cast<UTextBlock>(
				CurrentWidget->GetWidgetFromName(TEXT("StartButtonText"))))
			{
				ButtonText->SetText(FText::FromString(bIsRestart ? TEXT("RESTART") : TEXT("START")));
			}

			if (UTextBlock* ButtonText = Cast<UTextBlock>(
				CurrentWidget->GetWidgetFromName(TEXT("ExitButtonText"))))
			{
				ButtonText->SetText(FText::FromString(bIsRestart ? TEXT("MAIN MENU") : TEXT("EXIT")));
			}

			if (bIsRestart)
			{
				UFunction* PlayAnimFunc = CurrentWidget->FindFunction(FName("PlayGameOverAnim"));
				if (PlayAnimFunc)
				{
					CurrentWidget->ProcessEvent(PlayAnimFunc, nullptr);
				}

				if (UTextBlock* TotalScoreText = Cast<UTextBlock>(CurrentWidget->GetWidgetFromName("TotalScoreText")))
				{
					if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(UGameplayStatics::GetGameInstance(GetOwningPlayerController())))
					{
						TotalScoreText->SetText(FText::FromString(
							FString::Printf(TEXT("Total Score: %d"), SpartaGameInstance->TotalScore)
						));
					}
				}
			}
		}
	}
}

void ASpartaHUD::ShowGameHUD()
{
	ClearHUD();

	if (HUDWidgetClass)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), HUDWidgetClass);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();

			if (APlayerController* PlayerController = GetOwningPlayerController())
			{
				PlayerController->SetInputMode(FInputModeGameOnly());
				PlayerController->bShowMouseCursor = false;
			}
		}
	}
}

void ASpartaHUD::ClearHUD()
{
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}
}

void ASpartaHUD::UpdateHUD(float RemainingTime, int32 Score, int32 LevelIndex, int32 WaveIndex)
{
	if (!CurrentWidget) return;

	if (UTextBlock* TimeText = Cast<UTextBlock>(CurrentWidget->GetWidgetFromName(TEXT("Time"))))
	{
		TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
	}

	if (UTextBlock* ScoreText = Cast<UTextBlock>(CurrentWidget->GetWidgetFromName(TEXT("Score"))))
	{
		ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), Score)));
		// if (UGameInstance* GameInstance = GetGameInstance())
		// {
		// USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
		// ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), SpartaGameInstance->TotalScore)));
		// }
	}

	if (UTextBlock* LevelText = Cast<UTextBlock>(CurrentWidget->GetWidgetFromName(TEXT("Level"))))
	{
		LevelText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d"), LevelIndex + 1)));
	}

	if (UTextBlock* LevelText = Cast<UTextBlock>(CurrentWidget->GetWidgetFromName(TEXT("Wave"))))
	{
		LevelText->SetText(FText::FromString(FString::Printf(TEXT("Wave: %d"), WaveIndex + 1)));
	}
}
