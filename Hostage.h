// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Hostage.generated.h"

UENUM(BlueprintType)
enum class EHostageState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Following UMETA(DisplayName = "Following"),
	Safe UMETA(DisplayName = "Safe"),
	Dead UMETA(DisplayName = "Dead")
};

UCLASS()
class EXAMENPROJECT_API AHostage : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHostage();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Health system

    UFUNCTION(BlueprintCallable)
    bool IsAlive() const { return CurrentHealth > 0 && CurrentState != EHostageState::Dead; }

    UFUNCTION(BlueprintCallable)
    float GetHealthPercentage() const { return CurrentHealth / MaxHealth; }

    // Interaction
    UFUNCTION(BlueprintCallable)
    void StartFollowingPlayer(class APlayerCharacter* Player);

    UFUNCTION(BlueprintCallable)
    void SetSafe();

    UFUNCTION(BlueprintCallable)
    EHostageState GetHostageState() const { return CurrentState; }

protected:
    // State management
    UPROPERTY(BlueprintReadOnly, Category = Hostage)
    EHostageState CurrentState;

    // Health
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Health)
    float MaxHealth;

    UPROPERTY(BlueprintReadOnly, Category = Health, meta = (AllowPrivateAccess = "true"))
    float CurrentHealth;

    // Movement
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
    float FollowDistance;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
    float FollowSpeed;

    // References
    UPROPERTY()
    class APlayerCharacter* FollowingPlayer;

    // Behavior functions
    void IdleBehavior();
    void FollowingBehavior(float DeltaTime);
    void SafeBehavior();

    UFUNCTION(BlueprintImplementableEvent, Category = Animation)
    void OnDeath();

    UFUNCTION(BlueprintImplementableEvent, Category = Animation)
    void OnTakeDamage();

    UFUNCTION(BlueprintImplementableEvent, Category = Animation)
    void OnStartFollowing();

    UFUNCTION(BlueprintImplementableEvent, Category = Animation)
    void OnSetSafe();
};
