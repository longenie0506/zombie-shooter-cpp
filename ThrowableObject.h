// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThrowableObject.generated.h"

UCLASS()
class ZOMBIESHOOTER_API AThrowableObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AThrowableObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void StartThrow();

	void Explode();

	void ApplyDamageToNearbyActors();

	void SetExplosionDamage(float NewExplosionDamage);
	void SetExlosionRadius(float NewExplosionRadius);

	void DestroyThrowableObject();

	AController* GetOwnerController();


public: 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	class UAudioComponent* ExplosionSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UParticleSystemComponent* ExplosionEffect; // TSubclass

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* CollisionSphere;

	// Static Mesh to represent the grenade model
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* GrenadeMesh;
	
private:
	UPROPERTY()
	FTimerHandle TimeExplosion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade", meta = (AllowPrivateAccess = true))
	float ExplosionRadius = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade", meta = (AllowPrivateAccess = true))
	float ExplosionDamage = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade",meta=(AllowPrivateAccess=true))
	float FuseTime = 3.f;

	

	

};
