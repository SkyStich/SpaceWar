// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MatchPlayerStartBase.h"
#include "PointCapturePlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API APointCapturePlayerStart : public AMatchPlayerStartBase
{
	GENERATED_BODY()

	UFUNCTION(BlueprintPure)
	EPointNumber GetPointNumber() const { return PointNumber; }

private:
 
	UPROPERTY(EditAnywhere)
	EPointNumber PointNumber;
	
};
