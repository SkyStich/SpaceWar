// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseGrenadeRifleProjectile.h"
#include "GrenadeToTouchProjectile.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API AGrenadeToTouchProjectile : public ABaseGrenadeRifleProjectile
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;
	virtual void OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
};
