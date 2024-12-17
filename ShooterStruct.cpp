// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterStruct.h"

FShooterStruct::FShooterStruct()
{
	CharacterName = "";
	MaxHealth = 100.0f;
	Speed = 500.0f;
	ShooterType = EShooterType::Shooter;
}

FShooterStruct::~FShooterStruct()
{
}
