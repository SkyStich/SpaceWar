// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseThrowProjectile.h"
#include "Components/BoxComponent.h"
#include "SpaceWar/DataAssets/WeaponDataAsset.h"
#include "BaseMineProjectile.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API ABaseMineProjectile : public ABaseThrowProjectile
{
	GENERATED_BODY()

protected:

	UFUNCTION()
    void OnPointCaptureCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);

	virtual void BeginPlay() override;

public:

	ABaseMineProjectile();
	
	void Init(AController* OwnerController, FBaseThrowData* SpecialWeaponData);

protected:

	UPROPERTY(BlueprintReadOnly)
	AController* OwnerPlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* DetonationCollision;

private:

	FBaseThrowData* ThrowData;
};
