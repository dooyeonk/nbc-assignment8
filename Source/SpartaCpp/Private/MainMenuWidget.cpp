#include "MainMenuWidget.h"

#include "SpartaPlayerController.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnExitClicked);
	}
}

void UMainMenuWidget::UpdateExitButton(bool bIsInGame)
{
	if (ExitButtonText)
	{
		FString NewText = bIsInGame ? TEXT("MAIN MENU") : TEXT("EXIT");
		ExitButtonText->SetText(FText::FromString(NewText));
	}
}

void UMainMenuWidget::OnExitClicked()
{
	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			SpartaPlayerController->HandleExitButton();
		}
	}
}
