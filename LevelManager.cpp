// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h" 
#include "Blueprint/UserWidget.h"
#include "Engine/LevelStreaming.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "NavigationSystem.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "Kismet/GameplayStatics.h"
#include "ZS_PlayerController.h"
#include "ZS_GameMode.h"
#include "BaseCharacter.h"


ALevelManager::ALevelManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

FName ALevelManager::GetCurrentLevel()
{
	return CurrentLevel;
}

void ALevelManager::BeginPlay()
{
	Super::BeginPlay();

}

void ALevelManager::InitializeLevelManager()
{
	UE_LOG(LogTemp, Display, TEXT("Start Menu"));
	ShowStartMenu();
}

void ALevelManager::LoadLevel(FName LevelName)
{
	ULevelStreaming* StreamingLevel = UGameplayStatics::GetStreamingLevel(this, LevelName);
	if (StreamingLevel && StreamingLevel->IsLevelLoaded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Level already loaded: %s"), LevelName);
		return;
	}
	CurrentLevel = LevelName;
	// Show the loading screen
	HideStartMenu();
	ShowLoadingScreen();

	// Start streaming the level asynchronously
	UGameplayStatics::LoadStreamLevel(GetWorld(), LevelName, true, false, FLatentActionInfo());
	

	UE_LOG(LogTemp, Warning, TEXT("Loading level: %s"), TEXT("LevelName"));
	GetWorld()->GetTimerManager().SetTimer(LevelLoadTimerHandle, FTimerDelegate::CreateUObject(this, &ALevelManager::CheckLevelLoaded, LevelName), 2.0f, false);
}

void ALevelManager::UnloadLevel(FName LevelName)
{
	ULevelStreaming* StreamingLevel = UGameplayStatics::GetStreamingLevel(this, LevelName);
	if (StreamingLevel && !StreamingLevel->IsLevelLoaded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Level already unloaded: %s"), LevelName);
		return;
	}

	// Show the loading screen
	ShowLoadingScreen();
	

	// Start streaming the level asynchronously
	UGameplayStatics::UnloadStreamLevel(GetWorld(), LevelName,FLatentActionInfo(),false);

	UE_LOG(LogTemp, Warning, TEXT("Unloading level: %s"), LevelName);
	GetWorld()->GetTimerManager().SetTimer(LevelLoadTimerHandle, FTimerDelegate::CreateUObject(this, &ALevelManager::CheckLevelUnloaded, LevelName), 2.0f, false);
}

void ALevelManager::ShowLoadingScreen()
{
	if (LoadingScreenWidgetClass) {
		LoadingScreenWidget = CreateWidget<UUserWidget>(GetWorld(), LoadingScreenWidgetClass);
		if (LoadingScreenWidget) {
			LoadingScreenWidget->AddToViewport();
		}
	}
}

void ALevelManager::HideLoadingScreen()
{
	if (LoadingScreenWidget) {
		LoadingScreenWidget->RemoveFromParent();
	}
}

void ALevelManager::ShowStartMenu()
{
	if (StartMenuWidgetClass) {
		StartMenuWidget = CreateWidget<UUserWidget>(GetWorld(), StartMenuWidgetClass);
		if (StartMenuWidget) {
			SetUIInputMode();
			StartMenuWidget->AddToViewport();
		}
	}
}

void ALevelManager::HideStartMenu()
{
	if (StartMenuWidget) {
		RemoveUIInputMode();
		StartMenuWidget->RemoveFromParent();
	}
}

void ALevelManager::SetUIInputMode()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		FInputModeUIOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = true; // Ensure the cursor is visible
		AZS_PlayerController* ZSPlayerController = Cast<AZS_PlayerController>(PlayerController);
		if (ZSPlayerController) {
			ZSPlayerController->ControllerStop(); // Trigger start event in player controller
		}
	}
}

void ALevelManager::RemoveUIInputMode()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		FInputModeUIOnly InputMode;

		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = false; // Ensure the cursor is visible

		PlayerController->SetInputMode(FInputModeGameOnly());
		AZS_PlayerController* ZSPlayerController = Cast<AZS_PlayerController>(PlayerController);
		if (ZSPlayerController) {
			ZSPlayerController->ControllerStart(); // Trigger end event in player controller
		}

	}
}


void ALevelManager::CheckLevelLoaded(FName LevelName)
{
	ULevelStreaming* StreamingLevel = UGameplayStatics::GetStreamingLevel(this, LevelName);
	if (StreamingLevel->IsLevelLoaded()) {
		UE_LOG(LogTemp, Display, TEXT("loaded"));
		OnLoadLevelComplete();
		return;
	}
	else {
		UE_LOG(LogTemp, Display, TEXT("notloaded"));
		GetWorld()->GetTimerManager().SetTimer(LevelLoadTimerHandle, FTimerDelegate::CreateUObject(this, &ALevelManager::CheckLevelLoaded, LevelName), 2.0f, false);
	}
}

void ALevelManager::CheckLevelUnloaded(FName LevelName)
{
	ULevelStreaming* StreamingLevel = UGameplayStatics::GetStreamingLevel(this, LevelName);
	if (!StreamingLevel->IsLevelLoaded()) {
		OnUnloadLevelComplete();
		return;
	}
	else {
		GetWorld()->GetTimerManager().SetTimer(LevelLoadTimerHandle, FTimerDelegate::CreateUObject(this, &ALevelManager::CheckLevelUnloaded, LevelName), 2.0f, false);
	}
}

void ALevelManager::OnLoadLevelComplete()
{
	HideLoadingScreen();
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);

	AGameModeBase* CurrentGameMode = GetWorld()->GetAuthGameMode();
	if (CurrentGameMode)
	{
		AZS_GameMode* ZSGameMode = Cast<AZS_GameMode>(CurrentGameMode);
		if (ZSGameMode)
		{
			ZSGameMode->NewGame();
		}
	}

	if (PlayerStarts.Num() > 0)
	{
		APlayerStart* PlayerStart = Cast<APlayerStart>(PlayerStarts[0]);
		if (PlayerStart)
		{
			// Relocate the nav mesh for AIController works!
			UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
			if (NavSystem)
			{
				TArray<AActor*> NavMeshVolumes;

				UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANavMeshBoundsVolume::StaticClass(), NavMeshVolumes);

				if (NavMeshVolumes.Num() > 0)
				{
					ANavMeshBoundsVolume* FirstNavMeshVolume = Cast<ANavMeshBoundsVolume>(NavMeshVolumes[0]);

					if (FirstNavMeshVolume)
					{
						FVector NewScale(100.0f, 100.0f, 100.0f); 

						FirstNavMeshVolume->SetActorLocation(PlayerStart->GetActorLocation());
						FirstNavMeshVolume->SetActorScale3D(NewScale);
						NavSystem->Build();
					}
				}
				
			}

			// Set location for player
			APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
			if (PlayerController && PlayerController->GetPawn())
			{
				PlayerController->GetPawn()->SetActorLocation(PlayerStart->GetActorLocation());
				PlayerController->Possess(PlayerController->GetPawn());

				if (PlayerController->GetPawn()->IsValidLowLevel())
				{
					PlayerController->EnableInput(PlayerController);
				}
			}

		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No PlayerStart found in the level!"));
	}
	return;
}

void ALevelManager::OnUnloadLevelComplete()
{
	HideLoadingScreen();
	return;
}
