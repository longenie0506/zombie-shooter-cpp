// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponDataStruct.h"

FWeaponDataStruct::FWeaponDataStruct()
{
	Damage = 10.0f;
	FireRate = 0.5f;
	AmmoCapacity = 30;
	MaxAmmo = 120;
	ReloadTime = 2.0f;
	Range = 1000.f;
	WeaponType = EWeaponType::Rifle;
}

FWeaponDataStruct::~FWeaponDataStruct()
{
}
