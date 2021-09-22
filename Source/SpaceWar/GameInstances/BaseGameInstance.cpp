// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameInstance.h"


#include "Kismet/GameplayStatics.h"
#include "SpaceWar/SaveGame/SavePlayerData.h"

UBaseGameInstance::UBaseGameInstance()
{
	CurrentArmor = "Base";
	PlayerName = "PlayerName";
	
	Weapons.Add(EWeaponType::FirstWeapon, "SilverDragon");
	Weapons.Add(EWeaponType::SecondWeapon, "BlackJack");
}

void UBaseGameInstance::Init()
{
	Super::Init();
}

void UBaseGameInstance::SetCurrentArmor(const FName& Id)
{
	CurrentArmor = Id;
	SaveFullPlayerData();
}

/*TArray<FCurrentWeaponKey> UBaseGameInstance::GetWeaponsByPlayerClass()
{
	return Equipment.FindByPredicate([&](FEquipmentSave Value) -> bool { return CurrentSolderCategory == Value.Key; })->Value;
}*/

FName UBaseGameInstance::FindWeaponByType(EWeaponType Type, const TMap<EWeaponType, FName>& Map)
{
	return Map.FindRef(Type);
}

void UBaseGameInstance::ReplacementWeapon(EWeaponType Key, const FName& NewId)
{
	for(auto& ByArray : Weapons)
	{
		if(ByArray.Key == Key) ByArray.Value = NewId;
	}
	SaveFullPlayerData();
}

void UBaseGameInstance::LoadPlayerData()
{
	if(USavePlayerData* SaveGame = Cast<USavePlayerData>(UGameplayStatics::LoadGameFromSlot(PlayerName + "Data", 0)))
	{
		Weapons = SaveGame->WeaponData;
		CurrentArmor = SaveGame->EquippableArmorId;
	}
}

void UBaseGameInstance::SaveFullPlayerData()
{
	if(USavePlayerData* SavePlayerData = Cast<USavePlayerData>(UGameplayStatics::CreateSaveGameObject(USavePlayerData::StaticClass())))
	{
		SavePlayerData->WeaponData = Weapons;
		SavePlayerData->EquippableArmorId = CurrentArmor;
		UGameplayStatics::SaveGameToSlot(SavePlayerData, PlayerName + "Data", 0);
	}
}

void UBaseGameInstance::Shutdown()
{
	Super::Shutdown();
}

void UBaseGameInstance::SetPlayerName(const FString& NewPlayerName)
{
	PlayerName = NewPlayerName;
	LoadPlayerData();
}
