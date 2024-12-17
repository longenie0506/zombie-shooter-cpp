// Fill out your copyright notice in the Description page of Project Settings.


#include "ZS_PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "EnhancedInputSubsystems.h"

void AZS_PlayerController::BeginPlay()
{
	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
	// ControllerStart(); // Remove this line before shipping
}

void AZS_PlayerController::ControllerStart()
{
	if (PlayerHUDWidgetClass) {
		PlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(),PlayerHUDWidgetClass);
		if (PlayerHUDWidget) {
			PlayerHUDWidget->AddToViewport();
		}
	}
}

void AZS_PlayerController::ControllerStop()
{
	if (PlayerHUDWidget) {
		PlayerHUDWidget->RemoveFromParent();
	}
}
