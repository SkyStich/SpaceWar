// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "MatchPlayerCameraManager.generated.h"

UCLASS()
class SPACEWAR_API AMatchPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

private:

	UFUNCTION()
	void OnNewPawn(APawn* NewPawn);

	UFUNCTION()
	void OnOwnerSufferedDamage(const FVector& DamageDirection);

public:

	AMatchPlayerCameraManager();
	virtual void Tick(float DeltaSeconds) override;

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY()
	UMaterialInstanceDynamic* DamageDynamicInstance;

	UPROPERTY()
	UMaterial* DamageMaterial;

	UPROPERTY()
	TSubclassOf<UMatineeCameraShake> DamageCameraShakeClass;

	float DamageLerp;
};
