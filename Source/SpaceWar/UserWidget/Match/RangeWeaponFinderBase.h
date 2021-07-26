// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "RangeWeaponFinderBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class SPACEWAR_API URangeWeaponFinderBase : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "WeaponFinder")
	bool GetIsFinderActive() const { return IsFinderActive; }

	UFUNCTION(BlueprintCallable, Category = "WeaponFinder")
	void ClearChild(UScrollBox* ScrollBox);

	UFUNCTION(BlueprintCallable, Category = "WeaponFinder")
	void CreateSlot(UScrollBox* ScrollBox, class UAmmunitionWidgetBase* MainWidget, TSubclassOf<class UAmmunitionRangeWeaponSlotBase> SlotClass);

protected:

	UPROPERTY(BlueprintReadOnly)
	bool IsFinderActive;
};