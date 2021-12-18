// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameInstance.h"


#include "Kismet/GameplayStatics.h"
#include "SpaceWar/SaveGame/SavePlayerData.h"

UBaseGameInstance::UBaseGameInstance()
{
	CurrentArmor = "Base";
	PlayerName = "Mushu";
	CurrentThrowWeapon = "Boom";
	
	CurrentLevel = 1;
	CurrentExp = 0;
	
	Weapons.Add(EWeaponType::FirstWeapon, "SilverDragon");
	Weapons.Add(EWeaponType::SecondWeapon, "BlackJack");
}

void UBaseGameInstance::OnStart()
{
	Super::OnStart();
}

void UBaseGameInstance::Init()
{
	Super::Init();
}

void UBaseGameInstance::SetCurrentArmor(const FName& Id)
{
	CurrentArmor = Id;
}

FName UBaseGameInstance::FindWeaponByType(EWeaponType Type, const TMap<EWeaponType, FName>& Map)
{
	return Map.FindRef(Type);
}

void UBaseGameInstance::ReplacementWeapon(EWeaponType Key, const FName& NewId)
{
	if(Key == EWeaponType::FirstWeapon)
	{
		OnFirstWeaponChanged.Broadcast(NewId);
	}
	
	for(auto& ByArray : Weapons)
	{
		if(ByArray.Key == Key) ByArray.Value = NewId;
	}
}

void UBaseGameInstance::LoadPlayerData()
{
	//if(GIsServer) return;

	if(USavePlayerData* SaveGame = Cast<USavePlayerData>(UGameplayStatics::LoadGameFromSlot(FString(PlayerName + "Data"), GetPrimaryPlayerController()->NetPlayerIndex)))
	{
		UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("LoadArmor: %ls"), *SaveGame->EquippableArmorId.ToString()));
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
		UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("SaveArmor: %ls"), *CurrentArmor.ToString()));
		if(UGameplayStatics::SaveGameToSlot(SavePlayerData, FString(PlayerName + "Data"), GetPrimaryPlayerController()->NetPlayerIndex))
		{
			CurrentThrowWeapon = "Boom";
		}
	}
}

void UBaseGameInstance::Shutdown()
{
	Super::Shutdown();
}

void UBaseGameInstance::SetPlayerName(const FString& NewPlayerName)
{
	PlayerName = NewPlayerName;
}

void UBaseGameInstance::SetLevel(int32 Value)
{
	CurrentLevel = Value;
	OnCurrentLevelChanged.Broadcast(CurrentLevel);
}

void UBaseGameInstance::SetExp(int32 Value)
{
	CurrentExp = Value;
	OnCurrentExpChanged.Broadcast(CurrentExp, Value);
}


