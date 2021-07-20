// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpaceWar/PlayerControllers/Match/Base/MatchPlayerControllerBase.h"

#include "AmmunitionWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API UAmmunitionWidgetBase : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "AmmunitionWidget")
	void UpdateAmmo();

protected:

	UPROPERTY(BlueprintReadOnly)
	AMatchPlayerControllerBase* OwnerController;
};
