// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseMatchHUD.h"
#include "CaptureAndHoldHUD.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API ACaptureAndHoldHUD : public ABaseMatchHUD
{
	GENERATED_BODY()

	UFUNCTION()
	void PreparationSpawnCharacter();

protected:

	virtual void CreateSpectatorWidgets() override;
	virtual void BeginPlay() override;

public:

	ACaptureAndHoldHUD();
};
