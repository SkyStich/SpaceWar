// Fill out your copyright notice in the Description page of Project Settings.


#include "GrenadeLauncherBase.h"
#include "UObject/ConstructorHelpers.h"
#include "SpaceWar/SpaceWarCharacter.h"

UGrenadeLauncherBase::UGrenadeLauncherBase()
{
	static ConstructorHelpers::FClassFinder<AGrenadeToTouchProjectile>SecondaryProjectileFinder(TEXT("/Game/ThirdPersonCPP/Blueprints/Actors/Projectiles/GrenadeRifle/BP_GrenadeToTouch"));
	if(SecondaryProjectileFinder.Succeeded()) SecondaryProjectileClass = SecondaryProjectileFinder.Class;
}

void UGrenadeLauncherBase::ShotLogic()
{
	if(IsAuthority())
	{
		FActorSpawnParameters SpawnParam;
		SpawnParam.Owner = CharacterOwner->Controller;
		SpawnParam.Instigator = CharacterOwner;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FVector const Location = CharacterOwner->GetWeaponMesh()->GetSocketLocation("Muzzle");
		FRotator const Rotation = CharacterOwner->Controller->GetControlRotation();

		GetWorld()->SpawnActor<AGrenadeToTouchProjectile>(SecondaryProjectileClass, Location, Rotation, SpawnParam);
	}
}
