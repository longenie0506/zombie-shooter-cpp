// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieAIController.h"


#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BaseCharacter.h"


void AZombieAIController::BeginPlay()
{
    Super::BeginPlay();
}

void AZombieAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

bool AZombieAIController::IsDead() const
{
    ABaseCharacter* ControlledCharacter = Cast<ABaseCharacter>(GetPawn());
    if (ControlledCharacter != nullptr)
    {
        ControlledCharacter->Die();
    }

    return true;
}

void AZombieAIController::StartAI()
{
    if (AIBehavior != nullptr)
    {
        RunBehaviorTree(AIBehavior);

        APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

        if (PlayerPawn) {
            GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
        }


    }
}
