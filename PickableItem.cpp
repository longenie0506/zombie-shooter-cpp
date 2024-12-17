// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableItem.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "ShootingCharacter.h"

// Sets default values
APickableItem::APickableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    // Create the capsule component for collision detection
    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
    RootComponent = CapsuleComponent;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Meshcomponent"));
    MeshComponent->SetupAttachment(RootComponent);

    CapsuleComponent->SetCapsuleSize(42.f, 96.f); 
    // Enable collision
    CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    CapsuleComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

    // Bind the collision event
    CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &APickableItem::OnHit);

}

// Called when the game starts or when spawned
void APickableItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Handle collision with player
void APickableItem::OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Check if the other actor is the player (or player character)
    if (OtherActor && (OtherActor != this) && OtherActor->IsA(ABaseCharacter::StaticClass())) // Replace ABaseCharacter with your specific character class if needed
    {
        // Destroy the object
        Destroy();
    }
}

