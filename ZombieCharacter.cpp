// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieCharacter.h"
#include "ZombieAIController.h"

AZombieCharacter::AZombieCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	ZombieType = EZombieType::Melee;
}

void AZombieCharacter::BeginPlay()
{
	Super::BeginPlay();

    // Get the AIController if it’s not already assigned
    AZombieAIController* AIController = Cast<AZombieAIController>(GetController());

    if (AIController)
    {
        // If there is an AIController, possess the character (not necessary if AIController is already possessing)
        AIController->StartAI();

        // Optionally, call any other methods for AI initialization
        UE_LOG(LogTemp, Warning, TEXT("AIController has taken control of ZombieCharacter"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No AIController found!"));
    }
}

void AZombieCharacter::SpawnZombie(FVector SpawnLocation)
{

}

void AZombieCharacter::PerformMeleeAttack()
{
}

void AZombieCharacter::PerformRangeAttack()
{
}

void AZombieCharacter::PerformSwitchWeapon()
{
}

