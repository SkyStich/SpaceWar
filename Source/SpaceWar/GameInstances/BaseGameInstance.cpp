// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameInstance.h"

UBaseGameInstance::UBaseGameInstance()
{
	CurrentArmor = "Base";
	PlayerName = "PlayerBase";
	CurrentSolderCategory = ESolderCategory::Solder;
}

void UBaseGameInstance::Init()
{
	Super::Init();

	FEquipmentSave SaveTest;
	SaveTest.Key = ESolderCategory::Solder;
	SaveTest.Value.Add(EWeaponType::FirstWeapon, "SilverDragon");
	SaveTest.Value.Add(EWeaponType::SecondWeapon, "BlackJack");
	Equipment.Add(SaveTest);
}


void UBaseGameInstance::SetCurrentArmor(const FName& Id)
{
	CurrentArmor = Id;
}

bool UBaseGameInstance::GetWeapons(TMap<EWeaponType, FName>& ReturnMap)
{
	auto const Finder = Equipment.FindByPredicate([&](FEquipmentSave Value) -> bool
		{ return CurrentSolderCategory == Value.Key; });
	
	if(Finder)
	{
		ReturnMap = Finder->Value;
		return true;
	}
	return false;
}

FName UBaseGameInstance::FindWeaponByType(EWeaponType Type, const TMap<EWeaponType, FName>& Map)
{
	return Map.FindRef(Type);
}

void UBaseGameInstance::ReplacementWeapon(EWeaponType Key, const FName& NewId)
{
	auto const Finder = Equipment.FindByPredicate([&](FEquipmentSave& Value) -> bool
		{ return Value.Key == CurrentSolderCategory; });

	if(!Finder) return;

	for(auto& ByArray : Finder->Value)
	{
		if(ByArray.Key == Key) ByArray.Value = NewId;
	}
}