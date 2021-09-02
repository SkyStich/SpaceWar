// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PreparationCaptureHoldBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class SPACEWAR_API UPreparationCaptureHoldBase : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void SpawnPlayer();
};
