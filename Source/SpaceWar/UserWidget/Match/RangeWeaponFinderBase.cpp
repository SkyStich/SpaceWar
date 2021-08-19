// Fill out your copyright notice in the Description page of Project Settings.


#include "RangeWeaponFinderBase.h"
#include "SpaceWar/GameInstances/BaseGameInstance.h"
#include "AmmunitionRangeWeaponSlotBase.h"
#include "SpaceWar/UserWidget/Match/AmmunitionWidgetBase.h"
#include "AmmunitionWidgetBase.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "SpaceWar/Components/EquipableWeaponManager.h"

void URangeWeaponFinderBase::CreateSlot(UScrollBox* ScrollBox, UAmmunitionWidgetBase* MainWidget, TSubclassOf<UAmmunitionRangeWeaponSlotBase> SlotClass)
{
	/** return if tihs slot is active */
	if(IsFinderActive) return;
	
	auto const WeaponInInventory = MainWidget->GameInstanceBase->GetWeaponsByPlayerClass();

	IsFinderActive = true;
	
	for(const auto& ByArray :MainWidget->WeaponDataAsset->GetWeaponData())
	{
		if(WeaponInInventory.FindByPredicate([&](FCurrentWeaponKey& Value) -> bool{ return Value.Value == ByArray.Key; })) continue;

		auto const WidgetSlot = CreateWidget<UAmmunitionRangeWeaponSlotBase>(GetOwningPlayer(), SlotClass);

		WidgetSlot->Init(EWeaponType::Unknown, ByArray.Key);
		WidgetSlot->InitDesigner(MainWidget->WeaponDataAsset);
		WidgetSlot->IsInStock = true;
		WidgetSlot->OnSlotClicked.AddDynamic(MainWidget, &UAmmunitionWidgetBase::SlotClicked);

		ScrollBox->AddChild(WidgetSlot);
	}
}

void URangeWeaponFinderBase::ClearChild(UScrollBox* ScrollBox)
{
	ScrollBox->ClearChildren();
	IsFinderActive = false;
}

