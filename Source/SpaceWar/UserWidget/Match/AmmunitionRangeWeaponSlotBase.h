// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpaceWar/Components/EquipableWeaponManager.h"
#include "AmmunitionRangeWeaponSlotBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSlotClicked, class UAmmunitionRangeWeaponSlotBase*, ClickSlot);

UCLASS(Abstract)
class SPACEWAR_API UAmmunitionRangeWeaponSlotBase : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void Init(EWeaponType NewType, FName NewId);

	UFUNCTION(BlueprintImplementableEvent)
	void InitDesigner(UWeaponDataAsset* AssetData);

	UFUNCTION(BlueprintCallable)
	void SetType(EWeaponType NewType) { Type = NewType; }

	UFUNCTION(BlueprintPure)
	EWeaponType GetType() const { return Type; }
	
	UFUNCTION(BlueprintPure)
	FName GetId() const { return Id; }

protected:

	UPROPERTY(BlueprintReadOnly)
	FName Id;

	UPROPERTY(BlueprintReadOnly)
	EWeaponType Type;

public:

	/** true if player have not weapon with this id in inventory  */
	UPROPERTY(BlueprintReadOnly)
	bool IsInStock;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FSlotClicked OnSlotClicked;
};
