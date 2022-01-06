// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmunitionWidgetBase.h"
#include "SpaceWar/SpaceWarCharacter.h"
#include "AmmunitionRangeWeaponSlotBase.h"
#include "SpaceWar/Objects/Armor/Base/BaseArmorObject.h"
#include "Components/Spacer.h"
#include "Kismet/GameplayStatics.h"
#include "SpaceWar/Interfaces/UpdateAmmoInterface.h"

void UAmmunitionWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
}

void UAmmunitionWidgetBase::UpdateAmmo()
{
	if(GetOwningPlayerPawn()->GetClass()->ImplementsInterface(UUpdateAmmoInterface::StaticClass()))
	{
		IUpdateAmmoInterface::Execute_RefreshAmmo(GetOwningLocalPlayer());
	}
}

void UAmmunitionWidgetBase::InitThrow(UVerticalBox* ThrowBox, TSubclassOf<UAmmunitionThrowWidgetBase> SlotClass, FVector2D SpacerSize)
{
	/** Get armor id with game instance */
	if(!GameInstanceBase) return;

	auto const Throw = GameInstanceBase->GetCurrentThrowWeapon();

	auto const WidgetSlot = CreateWidget<UAmmunitionThrowWidgetBase>(GetOwningPlayer(), SlotClass);
	WidgetSlot->Init(Throw);
	WidgetSlot->InitDesigner(WeaponDataAsset);
	WidgetSlot->IsInStock = false;
	WidgetSlot->OnThrowSlotClicked.AddDynamic(this, &UAmmunitionWidgetBase::ThrowSlotClicked);

	/** add slot to vertical box */
	ThrowBox->AddChildToVerticalBox(WidgetSlot);

	/** Create Spacer and add to vertical box */
	auto const Spacer = NewObject<USpacer>(this);
	Spacer->SetSize(SpacerSize);
	ThrowBox->AddChildToVerticalBox(Spacer);
}

void UAmmunitionWidgetBase::InitArmor(UVerticalBox* ArmorBox, TSubclassOf<UAmmunitionArmorSlotBase> SlotClass, FVector2D SpacerSize)
{
	/** Get armor id with game instance */
	if(!GameInstanceBase) return;
	auto const Armor = GameInstanceBase->GetCurrentArmorId();

	auto const WidgetSlot = CreateWidget<UAmmunitionArmorSlotBase>(GetOwningPlayer(), SlotClass);
	WidgetSlot->Init(Armor);
	WidgetSlot->InitDesigner(ArmorDataAsset);
	WidgetSlot->IsInStock = false;
	WidgetSlot->OnArmorSlotClicked.AddDynamic(this, &UAmmunitionWidgetBase::ArmorSlotClicked);

	/** add slot to vertical box */
	ArmorBox->AddChildToVerticalBox(WidgetSlot);
	
	/** Create Spacer and add to vertical box */
    auto const Spacer = NewObject<USpacer>(this);
	Spacer->SetSize(SpacerSize);
	ArmorBox->AddChildToVerticalBox(Spacer);
}

void UAmmunitionWidgetBase::InitRangeWeapon(UVerticalBox* RangeWeaponBox, TSubclassOf<UAmmunitionRangeWeaponSlotBase> SlotClass, FVector2D SpacerSize)
{
	/** Get weapons id with game instance */
	if(!GameInstanceBase) return;
	auto const Weapons = GameInstanceBase->GetWeapons();
	
	for(const auto& ByArray : Weapons)
	{
		if(ByArray.Key == EWeaponType::SpecialWeapon) continue;

		/** Create slot widget */
		auto const WidgetSlot = CreateWidget<UAmmunitionRangeWeaponSlotBase>(GetOwningPlayer(), SlotClass);

		/** Call fun for initializer */
		WidgetSlot->Init(ByArray.Key, ByArray.Value);
		WidgetSlot->InitDesigner(WeaponDataAsset);
		WidgetSlot->IsInStock = false;
		WidgetSlot->SetType(ByArray.Key);
		WidgetSlot->OnSlotClicked.AddDynamic(this, &UAmmunitionWidgetBase::SlotClicked);

		/** add slot to vertical box */
		RangeWeaponBox->AddChildToVerticalBox(WidgetSlot);

		/** Create Spacer and add to vertical box */
		auto const Spacer = NewObject<USpacer>(this);
		Spacer->SetSize(SpacerSize);
		RangeWeaponBox->AddChildToVerticalBox(Spacer);
	}
}

void UAmmunitionWidgetBase::SlotClicked_Implementation(UAmmunitionRangeWeaponSlotBase* ClickSlot)
{
	if(ClickSlot->IsInStock)
	{
		if(SlotForReplacement == EWeaponType::Unknown || SlotForReplacement == EWeaponType::SpecialWeapon) return;

		auto const ArmorData = ArmorDataAsset->GetArmorData().Find(GameInstanceBase->GetCurrentArmorId());
		
		FName const TempWeaponCurrentId = GameInstanceBase->FindWeaponByType(SlotForReplacement == EWeaponType::FirstWeapon ? EWeaponType::SecondWeapon : EWeaponType::FirstWeapon, GameInstanceBase->GetWeapons());

		float const CurrentWeaponWeight = WeaponDataAsset->GetEquipWeaponData(TempWeaponCurrentId).WeaponCharacteristicsBase.Weight;
		float const NewWeaponWeight = WeaponDataAsset->GetEquipWeaponData(ClickSlot->GetId()).WeaponCharacteristicsBase.Weight;
		
		if((NewWeaponWeight + CurrentWeaponWeight) <= ArmorData->MaxWeight)
		{
			ASpaceWarCharacter* SpaceCharacter = Cast<ASpaceWarCharacter>(GetOwningPlayerPawn());
			GameInstanceBase->ReplacementWeapon(SlotForReplacement, ClickSlot->GetId());
			if(SpaceCharacter)
			{
				SpaceCharacter->GetWeaponManager()->Server_ReplacementWeapon(SlotForReplacement, ClickSlot->GetId());
			}
		}
		else
		{
			ClickSlot->SlotCanNotReplacement();
		}
	}
	else
	{
		SlotForReplacement = ClickSlot->GetType();
	}
}

void UAmmunitionWidgetBase::ArmorSlotClicked_Implementation(UAmmunitionArmorSlotBase* ClickSlot)
{
	if(ClickSlot->IsInStock)
	{
		float CurrentWeight = 0.f;
		for(auto const ByArray : GameInstanceBase->Weapons)
		{
			CurrentWeight += WeaponDataAsset->GetEquipWeaponData(ByArray.Value).WeaponCharacteristicsBase.Weight;
		}
		
		if(CurrentWeight > ArmorDataAsset->FindData(ClickSlot->Id).MaxWeight)
		{
			ClickSlot->ArmorCanNotReplacement();
			return;
		}
		ASpaceWarCharacter* SpaceCharacter = Cast<ASpaceWarCharacter>(GetOwningPlayerPawn());
		GameInstanceBase->SetCurrentArmor(ClickSlot->Id);
		if(SpaceCharacter)
		{
			SpaceCharacter->Server_ReplacementArmor(ClickSlot->Id);
		}
	}
}

void UAmmunitionWidgetBase::ThrowSlotClicked_Implementation(UAmmunitionThrowWidgetBase* ClickSlot)
{
	if(ClickSlot->IsInStock)
	{
		ASpaceWarCharacter* SpaceCharacter = Cast<ASpaceWarCharacter>(GetOwningPlayerPawn());
		GameInstanceBase->SetThrowWeapon(ClickSlot->Id);
		if(SpaceCharacter)
		{
			SpaceCharacter->Server_ReplacementThrow(ClickSlot->Id);
		}
	}
}

void UAmmunitionWidgetBase::StopUseState()
{
	auto const PC = GetOwningPlayer<AMatchPlayerControllerBase>();
	if(!PC)
	{
		UE_LOG(LogPlayerController, Error, TEXT("UAmmunitionWidgetBase::StopUseState --Cast to AMatchPlayerControllerBase is null"));
		return;
	}
	PC->StopUseAmmunitionState();
	PC->bShowMouseCursor = false;
	PC->SetInputMode(FInputModeGameOnly());
}
