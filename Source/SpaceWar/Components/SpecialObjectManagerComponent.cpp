// Fill out your copyright notice in the Description page of Project Settings.


#include "SpecialObjectManagerComponent.h"
#include "Net/UnrealNetwork.h"

USpecialObjectManagerComponent::USpecialObjectManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void USpecialObjectManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

