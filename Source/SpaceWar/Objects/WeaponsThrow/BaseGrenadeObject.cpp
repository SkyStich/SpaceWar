// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGrenadeObject.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SpaceWar/Actors/Match/Projectile/Throw/BaseGrenadeProjectile.h"
#include "SpaceWar/SpaceWarCharacter.h"

void UBaseGrenadeObject::GrenadeThrow(float const TotalTimeBeforeExplosion, bool bThrowSucceeded, AController* Controller)
{
	GetWorld()->GetTimerManager().ClearTimer(ExplosionHandle);
	StopUseWeapon();

	if(!IsAuthority()) return;
	
	FVector Start = FVector::ZeroVector;
	FRotator Rotation = FRotator::ZeroRotator;

	Start = CharacterOwner->GetWeaponMesh()->GetSocketLocation("Muzzle");
	Rotation = UKismetMathLibrary::ComposeRotators(Controller->GetControlRotation(), FRotator(0.f, 0.f, 25.f));

	FActorSpawnParameters SpawnParam;
	SpawnParam.Instigator = CharacterOwner;
	SpawnParam.Owner = Controller;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	auto const Grenade = GetWorld()->SpawnActor<ABaseGrenadeProjectile>(ThrowData.ProjectileClass, Start, Rotation, SpawnParam);
	if(Grenade)
	{
		Grenade->Init(&ThrowData, TotalTimeBeforeExplosion, bThrowSucceeded, CharacterOwner->Controller);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Grenade can not spawn."), *GetName());
	}
}

bool UBaseGrenadeObject::UseWeapon()
{
	if(Super::UseWeapon())
	{
		bWeaponUsed = true;
		bWantPreparation = true;
		return true;
	}
	return false;
}

void UBaseGrenadeObject::StopUseWeapon()
{
	/** We don't want to hold a grenade */
	bWantPreparation = false;

	/** if the timer that is responsible for preparing the grenade for throwing is not active. and the timer before the explosion is still active */
	if(!GetWorld()->GetTimerManager().IsTimerActive(PreparationForUseHandle))
	{
		//CharacterOwner->GetHealthComponent()->HealthEndedEvent.RemoveDynamic(this, &UMainGrenadeObject::OuterDead);
		if(GetWorld()->GetTimerManager().IsTimerActive(ExplosionHandle))
		{
			/** Throw the grenade and give it the remaining time to throw. To clarify that the owner threw a grenade. */
			GrenadeThrow(GetWorld()->GetTimerManager().GetTimerRemaining(ExplosionHandle), true, CharacterOwner->GetController());

			/** Prevent the object from controlling the timer before detonation */
			GetWorld()->GetTimerManager().ClearTimer(ExplosionHandle);
		}
		else
		{
			/** Call parent logic */
			Super::StopUseWeapon();	
		}
	}
}

void UBaseGrenadeObject::PreparationForUse()
{
	GetWorld()->GetTimerManager().ClearTimer(PreparationForUseHandle);
	if(CharacterOwner->Controller)
	{
		if(bWantPreparation)
		{
			/** if we held the grenade for too long and it exploded in the owner's hand */
			FTimerDelegate TimerDel;
			TimerDel.BindUObject(this, &UBaseGrenadeObject::GrenadeThrow, 0.f, true, CharacterOwner->Controller);
			GetWorld()->GetTimerManager().SetTimer(ExplosionHandle, TimerDel, ThrowData.TimeBeforeExp, false);
		}
		else
		{
			/** throw the grenade and give it the remaining time before detonating */
			GrenadeThrow(ThrowData.TimeBeforeExp, true, CharacterOwner->Controller);
		}
	}
}


