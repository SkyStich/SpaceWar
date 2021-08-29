// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuPlayerControllerBase.h"
#include "SpaceWar/BPFLibrary/ServerManipulationLibrary.h"

AMainMenuPlayerControllerBase::AMainMenuPlayerControllerBase()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	NetUpdateFrequency = 1.f;
}

void AMainMenuPlayerControllerBase::CreateServer_Implementation(const FString& MapReference, const FString& MapName, const FString& ServerName)
{
	UServerManipulationLibrary::LaunchServer(MapReference, "");
}
