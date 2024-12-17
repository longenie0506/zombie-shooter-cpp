// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponDataStruct.generated.h"

/**
 * 
 */
 // Enum for weapon types
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    Melee    UMETA(DisplayName = "Melee"),  // Melee attack
    Rifle    UMETA(DisplayName = "Rifle"),  // Shooting continually
    Grenade  UMETA(DisplayName = "Grenade") // Throwable
};

USTRUCT(BlueprintType)
struct ZOMBIESHOOTER_API FWeaponDataStruct
{
    GENERATED_BODY()
public:
	FWeaponDataStruct();
	~FWeaponDataStruct();
    
    // Weapon properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    FString WeaponName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float Damage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float FireRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    int32 AmmoCapacity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    int32 MaxAmmo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float ReloadTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float Range;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    EWeaponType WeaponType;
};
