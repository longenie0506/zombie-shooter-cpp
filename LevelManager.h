// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelManager.generated.h"


/**
 * 
 */

enum ELevelName
{
	Level1    UMETA(DisplayName = FName("Level1-NeonCity")), 
	Level2    UMETA(DisplayName = FName("Level2-Forglar")),  
	Level3    UMETA(DisplayName = FName("Level3-HellCage")), 
	Level4    UMETA(DisplayName = FName("Level4-GraveYard")) 
};

UCLASS()
class ZOMBIESHOOTER_API ALevelManager: public AActor
{
	GENERATED_BODY()
public:
	ALevelManager();

	UFUNCTION(BlueprintCallable)
	FName GetCurrentLevel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintCallable, Category = "Level Management")
	void InitializeLevelManager();

	UFUNCTION(BlueprintCallable, Category = "Level Management")
	void LoadLevel(FName LevelName);

	UFUNCTION(BlueprintCallable, Category = "Level Management")
	void UnloadLevel(FName LevelName);

	// Function to show the loading screen
	UFUNCTION(BlueprintCallable, Category = "Level Management")
	void ShowLoadingScreen();

	// Function to hide the loading screen
	UFUNCTION(BlueprintCallable, Category = "Level Management")
	void HideLoadingScreen();

	UFUNCTION(BlueprintCallable, Category = "Level Management")
	void ShowStartMenu();

	// Function to hide the loading screen
	UFUNCTION(BlueprintCallable, Category = "Level Management")
	void HideStartMenu();

	UFUNCTION(BlueprintCallable, Category = "Level Management")
	void SetUIInputMode();

	// Function to hide the loading screen
	UFUNCTION(BlueprintCallable, Category = "Level Management")
	void RemoveUIInputMode();

	// Reference to the loading screen widget
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> LoadingScreenWidgetClass;
	// Reference to the loading screen widget
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> StartMenuWidgetClass;


private:

	void CheckLevelLoaded(FName LevelName);

	void CheckLevelUnloaded(FName LevelName);

	void OnLoadLevelComplete();

	void OnUnloadLevelComplete();

	FName CurrentLevel;

private:
	// Timer handle to check level loading state
	FTimerHandle LevelLoadTimerHandle;

	

	// The actual instance of the loading screen widget
	UUserWidget* LoadingScreenWidget;

	// The actual instance of the loading screen widget
	UUserWidget* StartMenuWidget;
};
