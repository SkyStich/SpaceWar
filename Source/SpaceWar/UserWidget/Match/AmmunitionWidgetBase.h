// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "SpaceWar/DataAssets/WeaponDataAsset.h"
#include "SpaceWar/GameInstances/BaseGameInstance.h"
#include "SpaceWar/PlayerControllers/Match/Base/MatchPlayerControllerBase.h"
#include "AmmunitionWidgetBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class SPACEWAR_API UAmmunitionWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SlotClicked(class UAmmunitionRangeWeaponSlotBase* ClickSlot);

protected:

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void StopUseState();

	UFUNCTION(BlueprintCallable, Category = "AmmunitionWidget")
	void UpdateAmmo();

	UFUNCTION(BlueprintCallable, Category = "UserWidget")
	void InitRangeWeapon(UVerticalBox* RangeWeaponBox, TSubclassOf<class UAmmunitionRangeWeaponSlotBase> SlotClass, FVector2D SpacerSize = FVector2D(1, 100));

public:

	UPROPERTY(BlueprintReadWrite)
	UBaseGameInstance* GameInstanceBase;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "DataAsset")
	UWeaponDataAsset* WeaponDataAsset;
};
