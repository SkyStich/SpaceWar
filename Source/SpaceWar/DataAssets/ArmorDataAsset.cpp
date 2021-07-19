// Fill out your copyright notice in the Description page of Project Settings.


#include "ArmorDataAsset.h"
#include "SpaceWar/SpaceWarCharacter.h"
#include "SpaceWar/Singleton/BaseSingleton.h"
#include "SpaceWar/Objects/Armor/Base/BaseArmorObject.h"

UBaseArmorObject* UArmorDataAsset::SyncCreateArmorObject(UObject* WorldContext, FArmorData Data, ASpaceWarCharacter* Outer)
{
	if(!Data.ArmorClass.IsValid())
	{
		FString const InstigatorName = WorldContext ? WorldContext->GetFullName() : TEXT("Uncknown");
		UE_LOG(LogAssetData, Error, TEXT("UArmorDataAsset::SyncCreateArmorObject -- Asset ptr is null: %s"), *InstigatorName);
		return nullptr;
	}

	/** load to memory */
	FStreamableManager& StreamableManager = UBaseSingleton::Get().AssetLoader;
	FSoftObjectPath const Ref = Data.ArmorClass.ToSoftObjectPath();
	StreamableManager.LoadSynchronous(Ref);

	/** assets is memory */
	UClass* ArmorType = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), Outer, *Ref.ToString()));
	if(ArmorType == nullptr) return nullptr;

	auto const Armor = NewObject<UBaseArmorObject>(Outer, ArmorType);
	Armor->ArmorData = Data;

	return Armor;
}

bool UArmorDataAsset::CheckId(const FName& Id) const
{
	for(const auto& ByArray : ArmorDataMap)
	{
		if(Id == ByArray.Key) return true;
 	}
	return false;
}

