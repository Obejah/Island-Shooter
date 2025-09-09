// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Hostage.h"
#include "Blueprint/UserWidget.h"

class UUserWidget;

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize health values
	MaxHealth = 100.0f;
	Health = MaxHealth;
	IsAlive = true;
	
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(GEngine != nullptr);

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("CheckCheck playerchater is active"));

	CreateHUDWidget();
}
// Called to add the HUD to the viewport
void APlayerCharacter::CreateHUDWidget()
{
	if (PlayerHUD)
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		if (PlayerController)
		{
			// Create the widget instance - Use GetWorld() as the owning object
			HUDWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), PlayerHUD);

			if (HUDWidgetInstance)
			{
				HUDWidgetInstance->AddToViewport();

				HUDWidgetInstance->SetVisibility(ESlateVisibility::Visible);

				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("HUD Widget created and added to viewport"));
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Failed to create HUD Widget instance"));
			}
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("PlayerHUD not assigned"));
	}
}
// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	// Bind movement input
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

	// Bind Weapon & Interaction input
	EnhancedInputComponent->BindAction(FireWeaponAction, ETriggerEvent::Started, this, &APlayerCharacter::FireWeapon);
	EnhancedInputComponent->BindAction(FireWeaponAutoAction, ETriggerEvent::Triggered, this, &APlayerCharacter::FireWeaponAuto);
	EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &APlayerCharacter::Reload);
	EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &APlayerCharacter::Interact);
}


void APlayerCharacter::Move(const FInputActionValue& Value) 
{
	// Get movement vector 
	FVector2D MovementVector = Value.Get<FVector2D>();
	// Add movement in forward direction
	if (GetController() && (MovementVector.X != 0.0f || MovementVector.Y != 0.0f))
	{
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value) 
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (GetController())
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerCharacter::FireWeapon()
{
	 // Attempt to fire a projectile.
	if (ProjectileClass)
	{
		// Get the camera transform.
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		// Set MuzzleOffset to spawn projectiles slightly in front of the camera.
		MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

		// Transform MuzzleOffset from camera space to world space.
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

		// Can offset the bullet to be aimed slightly upwards.
		FRotator MuzzleRotation = CameraRotation;
		MuzzleRotation.Pitch += 0.0f;

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			// Spawn the projectile at the muzzle.
			AFPSProjectile* Projectile = World->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				// Set the projectile's initial trajectory.
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
			}
		}
	}
}

void APlayerCharacter::FireWeaponAuto(const FInputActionValue& Value)
{
	bool bIsPressed = Value.Get<bool>();
	if (bIsPressed)
	{

	}
	else
	{

	}
}

void APlayerCharacter::Reload()
{
	
}

void APlayerCharacter::Interact()
{
	// Cast a ray to detect what actor the player is interacting with
	FVector Start = GetActorLocation();
	FVector Forward = GetActorForwardVector();
	FVector End = Start + (Forward * 300.0f); 

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_PhysicsBody, CollisionParams);

	if (bHit)
	{

		// Check if target is hostage
		AHostage* HitHostage = Cast<AHostage>(HitResult.GetActor());
		if (HitHostage)
		{

			InteractWithHostage(HitHostage);
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Interacted with: ") + HitResult.GetActor()->GetName());
			return;
		}
	}
}

void APlayerCharacter::UpdateHealth(float value)
{
	Health = FMath::Clamp(value, 0.0f, MaxHealth);
	IsAlive = Health > 0;
	
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// Apply damage to health
	Health -= DamageAmount;
	Health = FMath::Max(0.0f, Health); // Clamp to minimum 0

	
	UUserWidget* SpecificHUDWidget = Cast<UUserWidget>(HUDWidgetInstance);

	if (SpecificHUDWidget)
	{
		FName FunctionName = FName(TEXT("DamageScreen"));
		if (SpecificHUDWidget->GetClass()->FindFunctionByName(FunctionName) != nullptr)
		{
			SpecificHUDWidget->ProcessEvent(SpecificHUDWidget->GetClass()->FindFunctionByName(FunctionName), nullptr);
		}
	}
	

	// Check if player died
	if (Health <= 0)
	{
		OnDeath();
	}

	return DamageAmount;
}
void APlayerCharacter::InteractWithHostage(AHostage* Hostage)
{
	if (Hostage)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Found hostage, state: ") + FString::FromInt((int)Hostage->GetHostageState()));
		if (Hostage->GetHostageState() == EHostageState::Idle)
		{
			Hostage->StartFollowingPlayer(this);
		}	
	}
}
void APlayerCharacter::OnDeath()
{
	IsAlive = false;
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		DisableInput(PlayerController);
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("DeathMessage"));
	UUserWidget* SpecificHUDWidget = Cast<UUserWidget>(HUDWidgetInstance);

	if (SpecificHUDWidget)
	{
		FName FunctionName = FName(TEXT("GameoverScreen"));
		if (SpecificHUDWidget->GetClass()->FindFunctionByName(FunctionName) != nullptr)
		{
			SpecificHUDWidget->ProcessEvent(SpecificHUDWidget->GetClass()->FindFunctionByName(FunctionName), nullptr);
		}
	}
}