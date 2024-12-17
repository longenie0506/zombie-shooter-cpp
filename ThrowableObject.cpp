// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowableObject.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "Engine/EngineTypes.h"
#include "Engine/DamageEvents.h"
#include "ZombieCharacter.h"

// Sets default values
AThrowableObject::AThrowableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->InitSphereRadius(ExplosionRadius);
	CollisionSphere->SetCollisionProfileName(TEXT("Trigger"));
	
	GrenadeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GrenadeMesh"));
	GrenadeMesh->SetSimulatePhysics(true);
	GrenadeMesh->SetEnableGravity(true);
	GrenadeMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	ExplosionEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExplosionEffect"));
	ExplosionEffect->SetVisibility(false);
	
	ExplosionSound = CreateDefaultSubobject<UAudioComponent>(TEXT("ExplosionSound"));
	
	RootComponent = GrenadeMesh;
	CollisionSphere->SetupAttachment(RootComponent);
	ExplosionEffect->SetupAttachment(RootComponent);
	ExplosionSound->SetupAttachment(RootComponent);

	ExplosionRadius = 300.f;
	ExplosionDamage = 100.f;
	FuseTime = 3.f;

}

// Called when the game starts or when spawned
void AThrowableObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AThrowableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AThrowableObject::StartThrow()
{
	GetWorld()->GetTimerManager().SetTimer(TimeExplosion, this, &AThrowableObject::Explode, FuseTime, false);
}

void AThrowableObject::Explode()
{
	if (ExplosionEffect)
	{
		ExplosionEffect->SetVisibility(true);
		ExplosionEffect->Activate();
	}

	if (ExplosionSound)
	{
		ExplosionSound->Play();
	}

	ApplyDamageToNearbyActors();

	GetWorld()->GetTimerManager().SetTimer(TimeExplosion, this, &AThrowableObject::DestroyThrowableObject, 1.5f, false); // 1.5 seconds delay

}

void AThrowableObject::ApplyDamageToNearbyActors()
{	
	//DrawDebugSphere(GetWorld(), CollisionSphere->GetComponentLocation(), ExplosionRadius, 12, FColor::Red, false, 5.0f);
	GrenadeMesh->SetVisibility(false);

	TArray<AActor*> OverlappingActors;
	CollisionSphere->GetOverlappingActors(OverlappingActors);

	FRadialDamageEvent RadialDamageEvent;

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor && Actor->IsA(AZombieCharacter::StaticClass()))
		{
			AZombieCharacter* ZombieCharacter = Cast<AZombieCharacter>(Actor);
			UE_LOG(LogTemp, Display, TEXT("Damage to: %s"),*Actor->GetName());
			

			AController* OwnerController = GetOwnerController();
			if (ZombieCharacter) {
				Actor->TakeDamage(ExplosionDamage, RadialDamageEvent, OwnerController, this);
			}
			
			//UGameplayStatics::ApplyDamage(Actor, ExplosionDamage, nullptr, this, nullptr);
		}
	}
}

void AThrowableObject::SetExplosionDamage(float NewExplosionDamage)
{
	ExplosionDamage = NewExplosionDamage;
}

void AThrowableObject::SetExlosionRadius(float NewExplosionRadius)
{
	ExplosionRadius = NewExplosionRadius;
}

void AThrowableObject::DestroyThrowableObject()
{
	Destroy();
}

AController* AThrowableObject::GetOwnerController()
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr)
		return nullptr;
	return OwnerPawn->GetController();
}

