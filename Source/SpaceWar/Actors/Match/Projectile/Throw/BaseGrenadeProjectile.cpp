// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGrenadeProjectile.h"

#include "SpaceWar/DataAssets/WeaponDataAsset.h"

// Sets default values
ABaseGrenadeProjectile::ABaseGrenadeProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
}

void ABaseGrenadeProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseGrenadeProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseGrenadeProjectile::Init(FBaseThrowData* SpecialWeaponData, float const TimeBeforeDetonation, bool bThrowSucceeded, AController* InstigatorController)
{
	if(GetLocalRole() != ROLE_Authority) return;

	/** If the throw was made, the player gives and impulse for the grenade otherwise the grenade falls to the place where the throw should have occurred */
	
	if(TimeBeforeDetonation <= 0)
	{
		Detonation(SpecialWeaponData->BaseDamage, SpecialWeaponData->ExpRange, InstigatorController);
	}
	else
	{
		FTimerDelegate TimerDel;
		TimerDel.BindUObject(this, &ABaseGrenadeProjectile::Detonation, SpecialWeaponData->BaseDamage, SpecialWeaponData->ExpRange, InstigatorController);

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, TimeBeforeDetonation, false);
	}
}

