// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponDataAsset.h"
#include "../Objects/Weapons/Base/RangeWeaponObjectBase.h"
#include "SpaceWar/Objects/WeaponsThrow/ThrowWeaponBase.h"
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

FBaseThrowData UWeaponDataAsset::GetThrowData(const FName& WeaponName) const
{
	return ThrowWeaponData.FindRef(WeaponName);
}

FEquipWeaponData UWeaponDataAsset::GetEquipWeaponData(const FName& WeaponName) const
{
	return EquipWeaponData.FindRef(WeaponName);
}

UThrowWeaponBase* UWeaponDataAsset::CreateThrowWeaponObject(const FName& WeaponName, UObject* WorldContext, UObject* Outer)
{
	auto const TempData = GetThrowData(WeaponName);
	auto const TempMesh = TempData.WeaponObject;

	if(TempMesh.IsNull())
	{
		FString const InstigatorName = WorldContext != nullptr ? *WorldContext->GetFullName() : TEXT("Unknown");
		UE_LOG(LogAssetData, Error, TEXT("UWeaponDataAsset::CreateThrowWeaponObject -- Asset ptr is null for : %s"), *InstigatorName);
		return nullptr;
	}

	/** Load asset to memory */
	FStreamableManager& AssetLoader = UBaseSingleton::Get().AssetLoader;
	FSoftObjectPath const Ref = TempMesh.ToSoftObjectPath();
	AssetLoader.LoadSynchronous(Ref);

	/** Load object and check that it has desired class */
	UClass* Type = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), Outer, *Ref.ToString()));

	if(Type == nullptr) return nullptr;

	auto const ObjectThrow = NewObject<UThrowWeaponBase>(Outer, Type);
	ObjectThrow->Init(TempData);
	return ObjectThrow;
}

URangeWeaponObjectBase* UWeaponDataAsset::CreateWeaponObject(const FName& WeaponName, UObject* WorldContext, UObject* Outer)
{
	auto const TempData = GetEquipWeaponData(WeaponName);
	auto const TempMesh = TempData.WeaponObject;
	if(TempMesh.IsNull())
	{
		FString const InstigatorName = WorldContext != nullptr ? *WorldContext->GetFullName() : TEXT("Unknown");
		UE_LOG(LogAssetData, Error, TEXT("UWeaponDataAsset::CreateWeaponObject -- Asset ptr is null for: %s"), *InstigatorName);
		return nullptr;
	}
	/** Load sync assert in memory */
	FStreamableManager& AssetLoader = UBaseSingleton::Get().AssetLoader;
	FSoftObjectPath const Reference = TempMesh.ToSoftObjectPath();
	AssetLoader.LoadSynchronous(Reference);

	//Now load object and check that it has desired class
	UClass* WeaponType = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), Outer, *Reference.ToString()));

	if(WeaponType == nullptr) return nullptr;

	auto const Weapon = NewObject<URangeWeaponObjectBase>(Outer, WeaponType);
	Weapon->Init(TempData);
	return Weapon;
}



