// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/DamageType.h"
#include "GameFramework/Actor.h"
#include "Engine/DamageEvents.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    Damage = 10.0f;
    FireRate = 0.5f;
    AmmoCapacity = 30;
    CurrentAmmo = AmmoCapacity;
    ReloadTime = 2.0f;
    Range = 1000.0f;
    WeaponType = EWeaponType::Rifle;


    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("Weapon Mesh");
    RootComponent = CapsuleComponent;
    WeaponMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::Reset()
{
    if (WeaponData) {
        Damage = WeaponData->WeaponData.Damage;
        FireRate = WeaponData->WeaponData.FireRate;
        AmmoCapacity = WeaponData->WeaponData.AmmoCapacity;
        ReloadTime = WeaponData->WeaponData.ReloadTime;
        Range = WeaponData->WeaponData.Range;
        WeaponType = WeaponData->WeaponData.WeaponType;

        CurrentAmmo = AmmoCapacity;
    }
}

void AWeapon::Fire()
{
    if (WeaponType == EWeaponType::Rifle){
        UE_LOG(LogTemp, Display, TEXT("a rifle"));
        RangeFire();
    }
    else if (WeaponType == EWeaponType::Melee) {
        UE_LOG(LogTemp, Display, TEXT("a melee"));
        MeleeAttack();
    }
    else if (WeaponType == EWeaponType::Grenade) {
        UE_LOG(LogTemp, Display, TEXT("a grenade"));
        ThrowFire();
    }
}

// Range weapon
void AWeapon::RangeFire()
{
    if (CurrentAmmo > 0) {
        CurrentAmmo--;
        UE_LOG(LogTemp, Display, TEXT("Rifle shoot: %d ammo left"),CurrentAmmo);

        
        

        FHitResult HitResult;
        FVector ShotDirection;
        /*if (ShootFlash) {
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShootFlash, WeaponMesh->GetComponentLocation(), ShotDirection.Rotation());
        }*/
        bool bSuccess = GunTrace(HitResult, ShotDirection);
        if (bSuccess)
        {
            if (ShootFlash) {
                UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShootFlash, HitResult.Location, ShotDirection.Rotation());
            }
            if (ShootSound) {
                UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShootSound, HitResult.Location);
            }

            AActor* HitActor = HitResult.GetActor();
            if (HitActor != nullptr)
            {   
                FPointDamageEvent DamageEvent(Damage, HitResult, ShotDirection, nullptr);
                AController* OwnerController = GetOwnerController();

                HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
            }
        }
    }
    if (!bIsReloading && CurrentAmmo == 0) {
        Reload();
    }
}

void AWeapon::ThrowFire()
{
    if (CurrentAmmo > 0) {
        CurrentAmmo--;
        UE_LOG(LogTemp, Display, TEXT("Throw grenade: %d ammo left"), CurrentAmmo);

        if (ThrowableObjectClass)
        {
            FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.0f; // Offset to prevent collision
            FRotator SpawnRotation = GetActorRotation();
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;           
            SpawnParams.Instigator = GetInstigator();

            AThrowableObject* ThrowableObject = GetWorld()->SpawnActor<AThrowableObject>(ThrowableObjectClass, SpawnLocation, SpawnRotation, SpawnParams);

            if (ThrowableObject)
            {
                ThrowableObject->SetExplosionDamage(Damage);
                ThrowableObject->StartThrow();

                //FVector ThrowDirection = GetActorForwardVector(); 
                FVector ThrowDirection = GetOwner()->GetActorForwardVector();
                FVector Impulse = ThrowDirection * Range;

                ThrowableObject->GrenadeMesh->AddImpulse(Impulse, NAME_None, true);

                //UPrimitiveComponent* RootComp = Cast<UPrimitiveComponent>(ThrowableObject->GetRootComponent());
                //if (RootComp)
                //{
                //    RootComp->AddImpulse(Impulse, NAME_None, true); // Apply impulse to the root component (CollisionSphere)
                //}
                
            }
        }
    }
    if (!bIsReloading && CurrentAmmo == 0) {
        Reload();
    }
}

void AWeapon::MeleeAttack()
{
    UE_LOG(LogTemp, Display, TEXT("Meele attack"));
    FHitResult HitResult;
    FVector ShotDirection;
    /*if (ShootFlash) {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShootFlash, WeaponMesh->GetComponentLocation(), ShotDirection.Rotation());
    }*/
    bool bSuccess = GunTrace(HitResult, ShotDirection);
    if (bSuccess)
    {
        if (ShootFlash) {
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShootFlash, HitResult.Location, ShotDirection.Rotation());
        }
        if (ShootSound) {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShootSound, HitResult.Location);
        }
        AActor* HitActor = HitResult.GetActor();
        if (HitActor != nullptr)
        {
            FPointDamageEvent DamageEvent(Damage, HitResult, ShotDirection, nullptr);
            AController* OwnerController = GetOwnerController();

            HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
        }
    }
}

void AWeapon::Reload()
{
    if (!bIsReloading) {
        bIsReloading = true;  // Mark as reloading to prevent firing or multiple reloads
        UE_LOG(LogTemp, Display, TEXT("Reloading..."));

        // Schedule the actual reload to happen after ReloadTime seconds
        GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AWeapon::CompleteReload, ReloadTime, false);
    }
}

int32 AWeapon::GetAmmoCapacity()
{
    return AmmoCapacity;
}

int32 AWeapon::GetCurrentAmmo()
{
    return CurrentAmmo;
}

void AWeapon::CompleteReload()
{
    bIsReloading = false;
    CurrentAmmo = AmmoCapacity;
    UE_LOG(LogTemp, Display, TEXT("Reloading complete: %d ammo loaded"), CurrentAmmo);
}

//// Weapon event

AController* AWeapon::GetOwnerController() const
{
    APawn* OwnerPawn = Cast<APawn>(GetOwner());
    if (OwnerPawn == nullptr)
        return nullptr;
    return OwnerPawn->GetController();
}

bool AWeapon::GunTrace(FHitResult& Hit, FVector& ShotDirection) {
    
    AController * OwnerController = GetOwnerController();
    if (OwnerController == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("No Owner Controller found!"));
        return false;
    }

    FVector Location;
    FRotator Rotation;
    OwnerController->GetPlayerViewPoint(Location, Rotation);
    ShotDirection = -Rotation.Vector();

    FVector End = Location + Rotation.Vector() * Range;

    UE_LOG(LogTemp, Log, TEXT("GunTrace Start Location: %s"), *Location.ToString());
    UE_LOG(LogTemp, Log, TEXT("GunTrace Shot Direction: %s"), *ShotDirection.ToString());
    UE_LOG(LogTemp, Log, TEXT("GunTrace End Location: %s"), *End.ToString());

    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);
    Params.AddIgnoredActor(GetOwner());

    // Draw debug line
    //if (GEngine) {
    //    // Draw a line in the editor (only during gameplay and when debugging)
    //    DrawDebugLine(GetWorld(), Location, End, FColor::Green, false, 1, 0, 1);
    //}

    bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);

    if (bHit)
    {
        UE_LOG(LogTemp, Log, TEXT("GunTrace Hit: %s at Location: %s"), *Hit.GetActor()->GetName(), *Hit.ImpactPoint.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("GunTrace Missed!"));
    }

    return bHit;
}

