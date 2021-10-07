// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"

#include "MiniMapBase.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API UMiniMapBase : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintPure)
	UImage* GetMapImage() const;
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintPure)
	float GetDevidion();
};
