// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "FPSProjectile.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class AHostage;

UCLASS()
class EXAMENPROJECT_API APlayerCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    APlayerCharacter();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerStats)
    float MaxHealth;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerStats)
    float Health;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerStats)
    bool IsAlive;

   

    // Input action declarations
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    UInputAction* LookAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    UInputAction* FireWeaponAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    UInputAction* FireWeaponAutoAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    UInputAction* ReloadAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    UInputAction* InteractAction;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    UInputMappingContext* DefaultMappingContext;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
    FVector MuzzleOffset;
    

    // Widget Reference
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI, meta = (AllowPrivateAccess = "true"))
    TSubclassOf<UUserWidget> PlayerHUD;
    UPROPERTY()
    UUserWidget* HUDWidgetInstance;

 



    // Projectile class to spawn.
    UPROPERTY(EditDefaultsOnly, Category = Projectile)
    TSubclassOf<class AFPSProjectile> ProjectileClass;

    // Input callback functions
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void FireWeapon();
    void FireWeaponAuto(const FInputActionValue& Value);
    void Reload();
    void Interact();
    void CreateHUDWidget();
    void OnDeath();
    
public:


    void UpdateHealth(float value);
    // Function to get health information for UI

    virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

    UFUNCTION(BlueprintCallable)
    float GetHealth() const { return Health; }

    UFUNCTION(BlueprintCallable)
    float GetMaxHealth() const { return MaxHealth; }

    UFUNCTION(BlueprintCallable)
    bool GetIsAlive() const { return IsAlive; }

    UFUNCTION(BlueprintCallable)
    void InteractWithHostage(AHostage* Hostage);

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};