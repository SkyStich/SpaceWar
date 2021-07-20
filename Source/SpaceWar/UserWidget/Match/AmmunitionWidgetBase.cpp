// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmunitionWidgetBase.h"
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
