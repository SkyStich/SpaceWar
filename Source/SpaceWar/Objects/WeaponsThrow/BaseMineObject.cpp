// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMineObject.h"
#include "SpaceWar/SpaceWarCharacter.h"
#include "SpaceWar/Actors/Match/Projectile/Throw/BaseMineProjectile.h"
#include "SpaceWar/Components/HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"

void UBaseMineObject::FindPositionForSettingsMine(AController* OwnerController)
{
	if(!IsAuthority())
	{
		StopUseWeapon();
		return;
	}
	FVector const TraceStart = CharacterOwner->GetCurrentFireTrace();
	FVector const TraceEnd = OwnerController->GetControlRotation().Vector() * 200.f + TraceStart; 
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Green, true, false, 2.f);

	/** drop horisontal trace for find object before character */
	FHitResult OutHit;
	FCollisionObjectQueryParams QueryParams;
	QueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	bool const bTraceSucceeded = GetWorld()->LineTraceSingleByObjectType(OutHit, TraceStart, TraceEnd, QueryParams);

	/** if hit actor is valid, drop vertical trace before hit actor, else drop vertical trace on trace end point */
	FVector VerticalTraceStart = bTraceSucceeded ? OutHit.ImpactPoint : OutHit.TraceEnd;
	VerticalTraceStart += FVector(0.f, 0.f, 6.f);
	
	/** drop trace down for find floor for setting of mine */
	FVector const VerticalTraceEnd = VerticalTraceStart - FVector(0.f, 0.f, 180.f);
	
	FHitResult VerticalHit;
	bool const bVerticalTraceSucceeded = GetWorld()->LineTraceSingleByObjectType(VerticalHit, VerticalTraceStart, VerticalTraceEnd, QueryParams);

	DrawDebugLine(GetWorld(), VerticalTraceStart, VerticalTraceEnd, FColor::Green, true, false, 2.f);
	DrawDebugSphere(GetWorld(), bVerticalTraceSucceeded ? VerticalHit.ImpactPoint : VerticalHit.TraceEnd, 12.f, 16, FColor::Yellow, true, false);

	if(bVerticalTraceSucceeded)
	{
		//SettingsMine
	}
	StopUseWeapon();
}

ABaseMineProjectile* UBaseMineObject::SettingsMine(AController* OwnerController, const FVector& SpawnLocation)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerController;
	SpawnParams.Instigator = CharacterOwner;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	FRotator const Rotation = CharacterOwner->GetCapsuleComponent()->GetComponentRotation();

	auto const SpawnMime = GetWorld()->SpawnActor<ABaseMineProjectile>(ThrowData.ProjectileClass, SpawnLocation, Rotation, SpawnParams);
	if(SpawnMime)
	{
		SpawnMime->Init(OwnerController, &ThrowData);
		return SpawnMime;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Mine can not spawn!"), *GetName());
	}
	return nullptr;
}

void UBaseMineObject::PreparationForUse()
{
	Super::PreparationForUse();
	
	FindPositionForSettingsMine(CharacterOwner->Controller);
}

void UBaseMineObject::OuterDead()
{
	GetWorld()->GetTimerManager().ClearTimer(PreparationForUseHandle);
}

bool UBaseMineObject::UseWeapon()
{
	return Super::UseWeapon();
}

void UBaseMineObject::StopUseWeapon()
{
	if(!GetWorld()->GetTimerManager().IsTimerActive(PreparationForUseHandle))
	{
		CharacterOwner->GetHealthComponent()->OnOwnerDead.RemoveDynamic(this, &UBaseMineObject::OuterDead);
		Super::StopUseWeapon();
	}
}

bool UBaseMineObject::IsAbleToUseWeapon()
{
	return Super::IsAbleToUseWeapon();
}