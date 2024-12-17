// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "ZombieCharacter.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EZombieType : uint8
{
	Melee    UMETA(DisplayName = "Melee"),  // Melee attack
	Rifle    UMETA(DisplayName = "Range"),  // Shooting continually
	Grenade  UMETA(DisplayName = "Boss") // Throwable
};

UCLASS()
class ZOMBIESHOOTER_API AZombieCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Set default values for zombie properties
	AZombieCharacter();

	// Override BeginPlay
	virtual void BeginPlay() override;

	// Specific behavior for spawning different zombies (melee, range, boss)
	void SpawnZombie(FVector SpawnLocation);

	// Methods to handle specific zombie behaviors
	virtual void PerformMeleeAttack();
	virtual void PerformRangeAttack();
	virtual void PerformSwitchWeapon();

private:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Zombie Stats",meta=(AllowPrivateAccess=true))
	EZombieType ZombieType;


};
