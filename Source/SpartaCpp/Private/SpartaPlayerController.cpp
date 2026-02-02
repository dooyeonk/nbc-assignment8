#include "SpartaPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "SpartaGameInstance.h"
#include "SpartaGameState.h"
#include "SpartaHUD.h"
#include "Kismet/GameplayStatics.h"

ASpartaPlayerController::ASpartaPlayerController()
	: InputMappingContext(nullptr), MoveAction(nullptr), JumpAction(nullptr), LookAction(nullptr),
	  SprintAction(nullptr)
{
}


void ASpartaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}

	FString CurrentMapName = GetWorld()->GetMapName();
	if (CurrentMapName.Contains("MenuLevel"))
	{
		if (ASpartaHUD* SpartaHUD = GetHUD<ASpartaHUD>())
		{
			SpartaHUD->ShowMainMenu(false);
		}
	}
}

void ASpartaPlayerController::StartGame()
{
	if (USpartaGameInstance* GameInstance = Cast<USpartaGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		GameInstance->CurrentLevelIndex = 0;
		GameInstance->TotalScore = 0;
	}

	UGameplayStatics::OpenLevel(GetWorld(), FName("BasicLevel"));
	SetPause(false);
}

void ASpartaPlayerController::HandleExitButton()
{
	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());

	FString MainMenuLevelName = TEXT("MenuLevel");

	bool bIsInGame = (CurrentLevelName != MainMenuLevelName);
	if (bIsInGame)
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName(*MainMenuLevelName));
	}
	else
	{
		ExitGame();
	}
}

void ASpartaPlayerController::ExitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), this, EQuitPreference::Quit, false);
}
