// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/SphereComponent.h"
#include "SpaceWar/Actors/Match/SpecialWeapon/SpecialWeaponObjectBase.h"
#include "SpecialFirstAidStation.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API ASpecialFirstAidStation : public ASpecialWeaponObjectBase
{
	GENERATED_BODY()

public:
	
	ASpecialFirstAidStation();

protected:

	virtual void BeginPlay() override;
	virtual void PlaceSucceeded() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);

	UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:

	UPROPERTY(EditAnywhere)
	float RegenerationValue;
	
protected:

	UPROPERTY(VisibleAnywhere)
	USphereComponent* FirstAidSphereComponent;
};
