// Fill out your copyright notice in the Description page of Project Settings.


#include "ZS_GameMode.h"
#include "LevelManager.h"
#include "Kismet/GameplayStatics.h"
#include "ZombieCharacter.h"
#include "PickableItem.h"
#include "Blueprint/UserWidget.h"
#include "ZS_PlayerController.h"
#include "BaseCharacter.h"

AZS_GameMode::AZS_GameMode()
{
    PrimaryActorTick.bCanEverTick = true;
}

ALevelManager* AZS_GameMode::GetLevelManager() const
{
	return LevelManager;
}

void AZS_GameMode::BeginPlay()
{
	Super::BeginPlay();
    UE_LOG(LogTemp, Display, TEXT("begin game mode"));
    
    // Spawn the LevelManager at runtime
    if (!LevelManager)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        LevelManager = GetWorld()->SpawnActor<ALevelManager>(ALevelManager::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
        if (LoadingScreenWidgetClass) {
            LevelManager->LoadingScreenWidgetClass = LoadingScreenWidgetClass;
        }
        if (StartMenuWidgetClass) {
            LevelManager->StartMenuWidgetClass = StartMenuWidgetClass;
        }
        
        LevelManager->InitializeLevelManager();
        
    }

    if (!LevelManager)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to spawn LevelManager!"));
        return;
    }
    
}

void AZS_GameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    SetZombieCount();
    SetKeyCount();

    if (CheckRemaining() and isPlaying)
    {
        ShowVictoryScreen();
    }

    if (CheckLose() and isPlaying) {
        ShowLoseScreen();
    }
}

void AZS_GameMode::NewGame() {
    TArray<AActor*> FoundZombies;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AZombieCharacter::StaticClass(), FoundZombies);

    TArray<AActor*> FoundKeys;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APickableItem::StaticClass(), FoundKeys);

    UE_LOG(LogTemp, Display, TEXT("%d"), FoundZombies.Num());
    MaxZombieNumber = FoundZombies.Num();
    MaxKeyNumber = FoundKeys.Num();

    CurrentKeyNumber = MaxKeyNumber;
    CurrentZombieNumber = MaxZombieNumber;

    SetIsPlaying(true);
}

void AZS_GameMode::SetZombieCount()
{
    TArray<AActor*> FoundZombies;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AZombieCharacter::StaticClass(), FoundZombies);

    CurrentZombieNumber = FoundZombies.Num();
}

void AZS_GameMode::SetKeyCount()
{
    TArray<AActor*> FoundKeys;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APickableItem::StaticClass(), FoundKeys);

    CurrentKeyNumber = FoundKeys.Num();
}

int64 AZS_GameMode::GetZombieCount()
{
    return CurrentZombieNumber;
}

int64 AZS_GameMode::GetKeyCount()
{
    return CurrentKeyNumber;
}

int64 AZS_GameMode::GetMaxZombieNumber()
{
    return MaxZombieNumber;
}

int64 AZS_GameMode::GetMaxKeyNumber()
{
    return MaxKeyNumber;
}

bool AZS_GameMode::CheckRemaining() const
{
    return CurrentZombieNumber == 0 && CurrentKeyNumber == 0;
}

bool AZS_GameMode::CheckLose() const
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

    if (PlayerController)
    {
        // Get the pawn controlled by this player
        APawn* PlayerPawn = PlayerController->GetPawn();

        if (PlayerPawn)
        {
            ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(PlayerPawn);
            if (BaseCharacter) {
                if (BaseCharacter->GetHealth() <= 0) {
                    return true;
                }
            }
        }
    }
    return false;
}

void AZS_GameMode::ShowVictoryScreen()
{
    SetIsPlaying(false);
    GetLevelManager()->SetUIInputMode();
    if (VictoryWidgetClass)
    {
        if (CurrentWidget)  
        {
            CurrentWidget->RemoveFromParent();
        }

        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), VictoryWidgetClass);
        if (CurrentWidget)
        {
            CurrentWidget->AddToViewport();
        }
    }
}

void AZS_GameMode::ShowLoseScreen()
{
    SetIsPlaying(false);
    GetLevelManager()->SetUIInputMode();
    if (VictoryWidgetClass)
    {
        if (CurrentWidget)
        {
            CurrentWidget->RemoveFromParent();
            
        }

        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), LoseWidgetClass);
        if (CurrentWidget)
        {
            CurrentWidget->AddToViewport();
        }
    }
}

void AZS_GameMode::HideCurrentScreen()
{
    if (CurrentWidget) {
        CurrentWidget->RemoveFromParent();
    }
}

bool AZS_GameMode::GetIsPlaying()
{
    return isPlaying;
}

void AZS_GameMode::SetIsPlaying(bool newIsPlaying)
{
    isPlaying = newIsPlaying;
}
