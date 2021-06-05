// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerDumpCapsule.h"

// Sets default values
APlayerDumpCapsule::APlayerDumpCapsule()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerDumpCapsule::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called to bind functionality to input
void APlayerDumpCapsule::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

