// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseGrenadeRifleProjectile.h"
#include "GrenadeStandartProjectile.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API AGrenadeStandartProjectile : public ABaseGrenadeRifleProjectile
{
	GENERATED_BODY()

public:
	AGrenadeStandartProjectile();
	
protected:

	virtual void BeginPlay() override;
	virtual void OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data", meta=(AllowPrivateAccess="true"))
	float TimeBeforeExplosion;
};
