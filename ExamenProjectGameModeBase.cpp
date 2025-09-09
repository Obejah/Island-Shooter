// Fill out your copyright notice in the Description page of Project Settings.


#include "ExamenProjectGameModeBase.h"

void AExamenProjectGameModeBase::StartPlay()
{
	Super::StartPlay();

	check(GEngine != nullptr);

	//Display een debug message voor 5 seconden
	// 
	GEngine->AddOnScreenDebugMessage(-1,  5.0f, FColor::Green, TEXT("test of het  werkt g"));
}