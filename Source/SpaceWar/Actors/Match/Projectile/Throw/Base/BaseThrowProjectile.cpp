// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseThrowProjectile.h"

// Sets default values
ABaseThrowProjectile::ABaseThrowProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	NetUpdateFrequency = 20;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
}

// Called when the game starts or when spawned
void ABaseThrowProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseThrowProjectile::Detonation_Implementation(float BaseDamage, float, AController* InstigatorController)
{
	
}


