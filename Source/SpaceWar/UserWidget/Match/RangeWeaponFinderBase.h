// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AmmunitionArmorSlotBase.h"
#include "AmmunitionThrowWidgetBase.h"
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
	
	UFUNCTION(BlueprintCallable, Category = "WeaponFinder")
	void CreateArmorSlots(UScrollBox* ScrollBox, class UAmmunitionWidgetBase* MainWidget, TSubclassOf<UAmmunitionArmorSlotBase> SlotClass);
		
	UFUNCTION(BlueprintCallable, Category = "WeaponFinder")
	void CreateThrowSlot(UScrollBox* ScrollBox, class UAmmunitionWidgetBase* MainWidget, TSubclassOf<UAmmunitionThrowWidgetBase> SlotClass);

protected:

	UPROPERTY(BlueprintReadOnly)
	bool IsFinderActive;
};
