// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpaceWar/DataAssets/WeaponDataAsset.h"

#include "AmmunitionThrowWidgetBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FThrowSlotClicked, class UAmmunitionThrowWidgetBase*, SlotClicked);

UCLASS()
class SPACEWAR_API UAmmunitionThrowWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
	public:

	void Init(FName NewId) { Id = NewId; }
	
	UFUNCTION(BlueprintImplementableEvent)
    void InitDesigner(UWeaponDataAsset* AssetData);

public:

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FThrowSlotClicked OnThrowSlotClicked;
	
	/** true if player have not weapon with this id in inventory  */
	UPROPERTY(BlueprintReadOnly)
	bool IsInStock;

	UPROPERTY(BlueprintReadOnly)
	FName Id;
};
