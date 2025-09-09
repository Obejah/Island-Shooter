// Fill out your copyright notice in the Description page of Project Settings.


#include "HostageSafeZone.h"
#include "Hostage.h"
#include "Components/BoxComponent.h"

// Sets default values
AHostageSafeZone::AHostageSafeZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionBox;
	CollisionBox->SetCollisionProfileName(TEXT("Trigger"));
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AHostageSafeZone::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AHostageSafeZone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHostageSafeZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHostageSafeZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Sees if hostage overlaps with the safe zone and sets its state to safe
	AHostage* Hostage = Cast<AHostage>(OtherActor);
	if (Hostage && Hostage->GetHostageState() == EHostageState::Following)
	{
		Hostage->SetSafe();
	}
}