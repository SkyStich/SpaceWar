// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmunitionWidgetBase.h"
#include "SpaceWar/SpaceWarCharacter.h"
#include "AmmunitionRangeWeaponSlotBase.h"
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

void UAmmunitionWidgetBase::InitRangeWeapon(UVerticalBox* RangeWeaponBox, TSubclassOf<UAmmunitionRangeWeaponSlotBase> SlotClass, FVector2D SpacerSize)
{
	/** Get weapons id with game instance */
	TMap<EWeaponType, FName> Weapons;
	if(!GameInstanceBase) return;
	if(GameInstanceBase->GetWeapons(Weapons))
	{
		for(const auto& ByArray : Weapons)
		{
			if(ByArray.Key == EWeaponType::SpecialWeapon) continue;

			/** Create slot widget */
			auto const WidgetSlot = CreateWidget<UAmmunitionRangeWeaponSlotBase>(GetOwningPlayer(), SlotClass);

			/** Call fun for initializer */
			WidgetSlot->Init(ByArray.Key, ByArray.Value);
			WidgetSlot->InitDesigner(WeaponDataAsset);
			WidgetSlot->IsInStock = false;
			WidgetSlot->OnSlotClicked.AddDynamic(this, &UAmmunitionWidgetBase::SlotClicked);

			/** add slot to vertical box */
			RangeWeaponBox->AddChildToVerticalBox(WidgetSlot);

			/** Create Spacer and add to vertical box */
			auto const Spacer = NewObject<USpacer>(this);
			Spacer->SetSize(SpacerSize);
			RangeWeaponBox->AddChildToVerticalBox(Spacer);
		}
	}
}

void UAmmunitionWidgetBase::SlotClicked_Implementation(UAmmunitionRangeWeaponSlotBase* ClickSlot)
{
	/** active slot type for replace */
	static EWeaponType SlotForReplacement;
	
	if(ClickSlot->IsInStock)
	{
		if(SlotForReplacement == EWeaponType::Unknown || SlotForReplacement == EWeaponType::SpecialWeapon) return;

		ASpaceWarCharacter* SpaceCharacter = Cast<ASpaceWarCharacter>(GetOwningPlayerPawn());
		if(!SpaceCharacter) return;

		GameInstanceBase->ReplacementWeapon(SlotForReplacement, ClickSlot->GetId());
		SpaceCharacter->GetWeaponManager()->Server_ReplacementWeapon(SlotForReplacement, ClickSlot->GetId());
	}
	else
	{
		SlotForReplacement = ClickSlot->GetType();
	}
}

void UAmmunitionWidgetBase::StopUseState()
{
	RemoveFromParent();
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
