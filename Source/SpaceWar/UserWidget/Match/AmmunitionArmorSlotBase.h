// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpaceWar/DataAssets/ArmorDataAsset.h"
#include "AmmunitionArmorSlotBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FArmorSlotClicked, class UAmmunitionArmorSlotBase*, SlotClicked);

UCLASS()
class SPACEWAR_API UAmmunitionArmorSlotBase : public UUserWidget
{
	GENERATED_BODY()

public:

	void Init(FName NewId) { Id = NewId; }
	
	UFUNCTION(BlueprintImplementableEvent)
	void InitDesigner(UArmorDataAsset* AssetData);
	
public:

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FArmorSlotClicked OnArmorSlotClicked;
	
	/** true if player have not weapon with this id in inventory  */
	UPROPERTY(BlueprintReadOnly)
	bool IsInStock;

	UPROPERTY(BlueprintReadOnly)
	FName Id;
};
