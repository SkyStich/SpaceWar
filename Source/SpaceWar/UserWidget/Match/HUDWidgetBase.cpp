// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDWidgetBase.h"


#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

void UHUDWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	OwnerPlayer = Cast<ASpaceWarCharacter>(GetOwningPlayerPawn());

	AmmoWidget = CreateWidget(GetOwningPlayer(), AmmoWidgetClass);
	AmmoWidget->AddToViewport();
}

void UHUDWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	UpdateAmmoSlot();
}

void UHUDWidgetBase::UpdateAmmoSlot()
{
	if(!OwnerPlayer && !AmmoWidget) return;
	
	auto const Weapon = OwnerPlayer->GetWeaponManager()->GetCurrentWeapon();
	if(Weapon)
	{
		if(!Weapon->GetWeaponUsed())
		{
			FVector2D ViewPortLocation;
			if(UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(GetOwningPlayer(), OwnerPlayer->GetWeaponMesh()->GetSocketLocation("SKT_AmmoWidget"), ViewPortLocation, false))
			{
				AmmoWidget->SetPositionInViewport(ViewPortLocation, false);
			}
		}
	}
}
