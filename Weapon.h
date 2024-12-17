// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponDataAsset.h"
#include "WeaponDataStruct.h" // This contains weapon type enum
#include "ThrowableObject.h"
#include "Weapon.generated.h"


UCLASS()
class ZOMBIESHOOTER_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	UWeaponDataAsset* WeaponData;

	// Weapon properties based on data asset
	float Damage;
	float FireRate;
	int32 AmmoCapacity;
	int32 CurrentAmmo;
	float ReloadTime;
	float Range;
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere)
	UParticleSystem* ShootFlash;

	UPROPERTY(EditAnywhere)
	USoundBase* ShootSound;



public:
	// For firsttime picked up or when change between weapons
	void Reset();

	// Master firing event
	void Fire();

	// For range weapon only
	void RangeFire();

	bool GunTrace(FHitResult& Hit, FVector& ShotDirection);
	AController* GetOwnerController() const;

	// For throwable weapon only
	void ThrowFire();

	// For melee weapon only
	void MeleeAttack();

	void Reload();

	UFUNCTION(BlueprintCallable)
	int32 GetAmmoCapacity();

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentAmmo();

private:
	FTimerHandle ReloadTimerHandle;
	bool bIsReloading = false;

	void CompleteReload();


public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UCapsuleComponent* CapsuleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throwable Object")
	TSubclassOf<AThrowableObject> ThrowableObjectClass;

	
};
