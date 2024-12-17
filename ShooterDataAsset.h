// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ShooterStruct.h"
#include "ShooterDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API UShooterDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Shooter")
	FShooterStruct ShooterData;
};
