// Fill out your copyright notice in the Description page of Project Settings.


#include "Hostage.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"

// Sets default values
AHostage::AHostage()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Initialize default values
    MaxHealth = 50.0f;
    CurrentHealth = MaxHealth;
    CurrentState = EHostageState::Idle;
    FollowDistance = 200.0f;
    FollowSpeed = 600.0f;

    // Disable collision with pawns to prevent pushing
    GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
}

// Called when the game starts or when spawned
void AHostage::BeginPlay()
{
	Super::BeginPlay();
    CurrentHealth = MaxHealth;

    GetCharacterMovement()->SetMovementMode(MOVE_Walking);
    GetCharacterMovement()->MaxWalkSpeed = FollowSpeed;
}


// Called every frame
void AHostage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (CurrentState == EHostageState::Dead) return;

    switch (CurrentState)
    {
    case EHostageState::Idle:
        IdleBehavior();
        break;
    case EHostageState::Following:
        FollowingBehavior(DeltaTime);
        break;
    case EHostageState::Safe:
        SafeBehavior();
        break;
    }
}

// Called to bind functionality to input
void AHostage::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AHostage::IdleBehavior()
{

}


void AHostage::FollowingBehavior(float DeltaTime)
{
    if (!FollowingPlayer) return;

    FVector PlayerLocation = FollowingPlayer->GetActorLocation();
    FVector MyLocation = GetActorLocation();

    // Calculate direction to player
    FVector DirectionToPlayer = (PlayerLocation - MyLocation).GetSafeNormal();

    // Calculate target location behind the player at follow distance
    FVector TargetLocation = PlayerLocation - (DirectionToPlayer * FollowDistance);

    // Calculate movement direction
    FVector MoveDirection = (TargetLocation - MyLocation).GetSafeNormal();

    // Move towards the target location
    AddMovementInput(MoveDirection);

    // Rotate to face the direction of movement
    if (MoveDirection.SizeSquared() > 0.1f)
    {
        FRotator TargetRotation = MoveDirection.Rotation();
        TargetRotation.Pitch = 0.0f;
        TargetRotation.Roll = 0.0f;
        SetActorRotation(TargetRotation);
    }
}

void AHostage::SafeBehavior()
{

}



void AHostage::StartFollowingPlayer(APlayerCharacter* Player)
{
    if (CurrentState != EHostageState::Idle) return;

    FollowingPlayer = Player;
    CurrentState = EHostageState::Following;

    // Enable movement
    GetCharacterMovement()->SetMovementMode(MOVE_Walking);
    GetCharacterMovement()->MaxWalkSpeed = FollowSpeed;

    OnStartFollowing();
}

void AHostage::SetSafe()
{
    if (CurrentState != EHostageState::Following) return;

    CurrentState = EHostageState::Safe;
    FollowingPlayer = nullptr;

    // Stop movement
    GetCharacterMovement()->DisableMovement();

    OnSetSafe();
}

float AHostage::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    // Hostages can only take damage when idle
    if (CurrentState != EHostageState::Idle)
    {
        return 0.0f; // No damage taken
    }

    // Prevents negative damage numbers
    if (DamageAmount <= 0.0f)
    {
        return 0.0f;
    }

    // Apply damage
    CurrentHealth -= DamageAmount;
    CurrentHealth = FMath::Max(0.0f, CurrentHealth);


    // Check for death
    if (CurrentHealth <= 0.0f && CurrentState != EHostageState::Dead)
    {
        CurrentState = EHostageState::Dead;
        // Disable movement and collision
        GetCharacterMovement()->DisableMovement();
        GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        // Trigger death event
        OnDeath();
        Destroy();

    }

    // Call parent implementation
    return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}