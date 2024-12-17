// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterStruct.generated.h"
/**
 * 
 */
 // Enum for shooter types
UENUM(BlueprintType)
enum class EShooterType : uint8
{
    Shooter    UMETA(DisplayName = "Shooter"), 
    MeleeZombie    UMETA(DisplayName = "MeleeZombie"),
    RangeZombie    UMETA(DisplayName = "RangeZombie"),  
    BossZombie  UMETA(DisplayName = "BossZombie") 
};

USTRUCT(BlueprintType)
struct ZOMBIESHOOTER_API FShooterStruct
{
    GENERATED_BODY()
public:
	FShooterStruct();
	~FShooterStruct();

    // Weapon properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    FString CharacterName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float MaxHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float Speed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    EShooterType ShooterType;
};