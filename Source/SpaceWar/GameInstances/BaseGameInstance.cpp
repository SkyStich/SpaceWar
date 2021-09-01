// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameInstance.h"

UBaseGameInstance::UBaseGameInstance()
{
	CurrentArmor = "Base";
	PlayerName = "PlayerName";
	CurrentSolderCategory = ESolderCategory::Solder;
}

void UBaseGameInstance::Init()
{
	Super::Init();

	FEquipmentSave SaveTest;
	SaveTest.Key = ESolderCategory::Solder;
	SaveTest.Value.Add(FCurrentWeaponKey(EWeaponType::FirstWeapon, "SilverDragon"));
	SaveTest.Value.Add(FCurrentWeaponKey(EWeaponType::SecondWeapon, "BlackJack"));
	Equipment.Add(SaveTest);
}

void UBaseGameInstance::SetCurrentArmor(const FName& Id)
{
	CurrentArmor = Id;
}

TArray<FCurrentWeaponKey> UBaseGameInstance::GetWeaponsByPlayerClass()
{
	return Equipment.FindByPredicate([&](FEquipmentSave Value) -> bool
		{ return CurrentSolderCategory == Value.Key; })->Value;
}

FName UBaseGameInstance::FindWeaponByType(EWeaponType Type, const TMap<EWeaponType, FName>& Map)
{
	return Map.FindRef(Type);
}

void UBaseGameInstance::ReplacementWeapon(EWeaponType Key, const FName& NewId)
{
	 auto Finder = &Equipment.FindByPredicate([&](FEquipmentSave& Value) -> bool
		{ return Value.Key == CurrentSolderCategory; })->Value;

	for(auto& ByArray : *Finder)
	{
		if(ByArray.Key == Key)
		{
			ByArray.Value = NewId;
		}
	}
}
