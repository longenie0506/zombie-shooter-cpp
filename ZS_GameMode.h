// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ZS_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API AZS_GameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
    AZS_GameMode();

    // Get the LevelManager instance
    UFUNCTION(BlueprintCallable, Category = "Level Management")
    ALevelManager* GetLevelManager() const;

    UFUNCTION(BlueprintCallable, Category = "Zombie Gamemode")
    void NewGame();

    void SetZombieCount();
    void SetKeyCount();
    UFUNCTION(BlueprintCallable, Category = "Zombie Gamemode")
    int64 GetZombieCount();
    UFUNCTION(BlueprintCallable, Category = "Zombie Gamemode")
    int64 GetKeyCount();
    UFUNCTION(BlueprintCallable, Category = "Zombie Gamemode")
    int64 GetMaxZombieNumber();
    UFUNCTION(BlueprintCallable, Category = "Zombie Gamemode")
    int64 GetMaxKeyNumber();


    UFUNCTION(BlueprintCallable, Category = "Zombie Gamemode")
    bool CheckRemaining() const;

    UFUNCTION(BlueprintCallable, Category = "Zombie Gamemode")
    bool CheckLose() const;
    
    UFUNCTION(BlueprintCallable, Category = "Zombie Gamemode")
    void ShowVictoryScreen();

    UFUNCTION(BlueprintCallable, Category = "Zombie Gamemode")
    void ShowLoseScreen();

    UFUNCTION(BlueprintCallable, Category = "Zombie Gamemode")
    void HideCurrentScreen();

    UFUNCTION(BlueprintCallable, Category = "Zombie Gamemode")
    bool GetIsPlaying();
    UFUNCTION(BlueprintCallable, Category = "Zombie Gamemode")
    void SetIsPlaying(bool newIsPlaying);
    

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<class UUserWidget> VictoryWidgetClass;

    UUserWidget* CurrentWidget;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<class UUserWidget> LoseWidgetClass;


public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category = "Level Manager", meta = (AllowPrivateAccess = "true"))
    ALevelManager* LevelManager;

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> LoadingScreenWidgetClass;

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> StartMenuWidgetClass;


    int64 MaxZombieNumber;
    int64 MaxKeyNumber;

    int64 CurrentZombieNumber;
    int64 CurrentKeyNumber;

    bool isPlaying = false;


};
