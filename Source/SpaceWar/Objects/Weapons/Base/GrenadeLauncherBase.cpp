// Fill out your copyright notice in the Description page of Project Settings.


#include "GrenadeLauncherBase.h"
#include "UObject/ConstructorHelpers.h"
#include "SpaceWar/SpaceWarCharacter.h"

UGrenadeLauncherBase::UGrenadeLauncherBase()
{
	static ConstructorHelpers::FClassFinder<AGrenadeToTouchProjectile>SecondaryProjectileFinder(TEXT("/Game/ThirdPersonCPP/Blueprints/Actors/Projectiles/GrenadeRifle/BP_GrenadeToTouch"));
	if(SecondaryProjectileFinder.Succeeded()) SecondaryProjectileClass = SecondaryProjectileFinder.Class;
	
	static ConstructorHelpers::FClassFinder<AGrenadeStandartProjectile>BaseProjectileFinder(TEXT("/Game/ThirdPersonCPP/Blueprints/Actors/Projectiles/GrenadeRifle/BP_GrenadeStandartProjectile"));
	if(BaseProjectileFinder.Succeeded()) FirstlyProjectileClass = BaseProjectileFinder.Class;
}

void UGrenadeLauncherBase::ShotLogic()
{
	if(IsAuthority())
	{
		FActorSpawnParameters SpawnParam;
		SpawnParam.Owner = CharacterOwner->Controller;
		SpawnParam.Instigator = CharacterOwner;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		TSubclassOf<ABaseGrenadeRifleProjectile> CurrentProjectileClass;
		if(bPrimaryFiringMode) CurrentProjectileClass = FirstlyProjectileClass;
		else CurrentProjectileClass = SecondaryProjectileClass;

		FVector const Location = CharacterOwner->GetWeaponMesh()->GetSocketLocation("Muzzle");
		FRotator const Rotation = CharacterOwner->Controller->GetControlRotation();

		GetWorld()->SpawnActor<ABaseGrenadeRifleProjectile>(CurrentProjectileClass, Location, Rotation, SpawnParam);
	}
}
