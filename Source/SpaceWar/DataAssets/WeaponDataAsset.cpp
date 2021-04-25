// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponDataAsset.h"
#include "../Objects/Weapons/Base/BaseWeaponObject.h"
#include "../Singleton/BaseSingleton.h"

USkeletalMesh* UWeaponDataAsset::GetWeaponMesh(TAssetPtr<USkeletalMesh> ItemMesh) const
{
	if(ItemMesh.IsPending())
	{
		auto const SoftObject = ItemMesh.ToSoftObjectPath();

		ItemMesh = Cast<USkeletalMesh>(UBaseSingleton::Get().AssetLoader.LoadSynchronous(SoftObject));
	}
	return ItemMesh.Get();
}

FEquipWeaponData UWeaponDataAsset::GetEquipWeaponData(const FName& WeaponName) const
{
	return EquipWeaponData.FindRef(WeaponName);
}

UBaseWeaponObject* UWeaponDataAsset::CreateWeaponObject(const FName& WeaponName, UObject* WorldContext, UObject* Outer)
{
	auto const TempData = GetEquipWeaponData(WeaponName);
	auto const TempMesh = TempData.WeaponObject;
	if(TempMesh.IsNull())
	{
		FString const InstigatorName = WorldContext != nullptr ? *WorldContext->GetFullName() : TEXT("Unknown");
		UE_LOG(LogAssetData, Error, TEXT("UMyAssetLibrary::SyncCreateObject -- Asset ptr is null for: %s"), *InstigatorName);
		return nullptr;
	}
	/** Load sync assert in memory */
	FStreamableManager& AssetLoader = UBaseSingleton::Get().AssetLoader;
	FSoftObjectPath const Reference = TempMesh.ToSoftObjectPath();
	AssetLoader.LoadSynchronous(Reference);

	//Now load object and check that it has desired class
	UClass* WeaponType = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), Outer, *Reference.ToString()));

	if(WeaponType == nullptr) return nullptr;

	auto const Weapon = NewObject<UBaseWeaponObject>(Outer, WeaponType);
	Weapon->Init(TempData);
	return Weapon;
}



