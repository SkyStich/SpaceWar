// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "SpaceWar/Actors/Match/Projectile/Throw/Base/BaseThrowProjectile.h"
#include "BaseGrenadeProjectile.generated.h"

UCLASS()
class SPACEWAR_API ABaseGrenadeProjectile : public ABaseThrowProjectile
{
	GENERATED_BODY()
	
public:	

	ABaseGrenadeProjectile();

	virtual void Tick(float DeltaTime) override;

	void Init(struct FBaseThrowData* SpecialWeaponData, float const TimeBeforeDetonation, bool bThrowSucceeded, AController* InstigatorController);

protected:

	virtual void BeginPlay() override;


protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* ProjectileMovementComponent;
};
