// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	//FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	//FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->TargetArmLength = 300.f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bDoCollisionTest = true;
	SpringArm->ProbeSize = 5.0f;

	SpringArm->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(SpringArm);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Health = 100.0f;
	MaxHealth = 100.0f;
	Speed = 500.0f;
	ShooterType = EShooterType::Shooter;
	CharacterName = TEXT("BaseCharacter");
	// Status instructions:
	// 0: Dead
	// 1: Active/Alive
	Status = 1;

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (ShooterData) {
		CharacterName = ShooterData->ShooterData.CharacterName;
		MaxHealth = ShooterData->ShooterData.MaxHealth;
		Speed = ShooterData->ShooterData.Speed;
		ShooterType = ShooterData->ShooterData.ShooterType;
	}

	USkeletalMeshComponent* SkeletalMeshComp = GetMesh();

	// Check if the mesh component is valid
	if (SkeletalMeshComp)
	{
		// Hide the "weapon_r" bone (right-hand weapon) with no physics simulation
		SkeletalMeshComp->HideBoneByName(FName("weapon_r"), EPhysBodyOp::PBO_None);
	}

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}

		PlayerController->SetViewTarget(this);
	}

	if (WeaponClasses.IsValidIndex(0)) {
		SwitchWeapon(0);
	}
	
}

void ABaseCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ABaseCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(-LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCharacter(DeltaTime);
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Look);

		// Shooting and Aiming
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &ABaseCharacter::FireWeapon);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &ABaseCharacter::Aim);
		EnhancedInputComponent->BindAction(UnaimAction, ETriggerEvent::Completed, this, &ABaseCharacter::Unaim);
		EnhancedInputComponent->BindAction(Change1Action, ETriggerEvent::Started, this, &ABaseCharacter::SelectWeapon1);
		EnhancedInputComponent->BindAction(Change2Action, ETriggerEvent::Started, this, &ABaseCharacter::SelectWeapon2);
		EnhancedInputComponent->BindAction(Change3Action, ETriggerEvent::Started, this, &ABaseCharacter::SelectWeapon3);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

float ABaseCharacter::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Status==1) {
		Health -= DamageAmount;
		if (Health <= 0) {
			Die();
		}
	}

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ABaseCharacter::Die()
{	
	Health = 0;
	Status = 0;
	UE_LOG(LogTemp, Display, TEXT("%s has died!"),*CharacterName);
	if (ShooterType!=EShooterType::Shooter) {
		if (EquippedWeapon) {
			EquippedWeapon->Destroy();
		}
		if (DeathEffect) {
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathEffect, GetActorLocation());
		}
		
		Destroy();
	}
}

float ABaseCharacter::GetHealth() const
{
	return Health;
}

void ABaseCharacter::UpdateCharacter(float DeltaTime)
{
}

AWeapon* ABaseCharacter::GetEquippedWeapon()
{
	return EquippedWeapon;
}

void ABaseCharacter::FireWeapon()
{
	if (EquippedWeapon) {
		EquippedWeapon->Fire();
	}
}


// Range weapon
void ABaseCharacter::RangeFire()
{
	
}

void ABaseCharacter::ThrowFire()
{
	
}

void ABaseCharacter::MeleeAttack()
{
	
}

void ABaseCharacter::Aim()
{	
	OriginArmLength = SpringArm->TargetArmLength;
	SpringArm->TargetArmLength = -50.0f;
	UE_LOG(LogTemp, Display, TEXT("Aim"));
}

void ABaseCharacter::Unaim()
{
	SpringArm->TargetArmLength = OriginArmLength;
	UE_LOG(LogTemp, Display, TEXT("UnAim"));
}

void ABaseCharacter::SwitchWeapon(int32 WeaponIndex)
{
	if (WeaponClasses.IsValidIndex(WeaponIndex) && WeaponIndex!= CurrentWeaponIndex) {
		if (EquippedWeapon) {
			EquippedWeapon->Destroy();
			EquippedWeapon = nullptr;
		}

		CurrentWeaponIndex = WeaponIndex;
		EquippedWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClasses[WeaponIndex]);
		if (EquippedWeapon) {
			EquippedWeapon->Reset();
			FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
			EquippedWeapon->AttachToComponent(GetMesh(), AttachRules, TEXT("hand_r"));
			EquippedWeapon->SetOwner(this);
		}
	}
}

void ABaseCharacter::SelectWeapon1()
{
	SwitchWeapon(0);
}

void ABaseCharacter::SelectWeapon2()
{
	SwitchWeapon(1);
}

void ABaseCharacter::SelectWeapon3()
{
	SwitchWeapon(2);
}

