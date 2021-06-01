// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SpaceWar/Actors/Match/Projectile/Throw/Base/BaseThrowProjectile.h"
#include "WeaponDataAsset.generated.h"

class URangeWeaponObjectBase;
class UThrowWeaponBase;

UENUM(BlueprintType)
enum class EWeaponCategory : uint8
{
	Rifle,
	Pistol,
	Special
};

USTRUCT(BlueprintType)
struct FBaseWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|Description")
	FName WeaponName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|WeaponParam")
	float BaseDamage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|WeaponParam")
	float DelayBeforeUse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|Base")
	TAssetPtr<USkeletalMesh> ItemMesh;
};

USTRUCT()
struct FBaseThrowData : public FBaseWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|WeaponParam")
	float ExpRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|WeaponParam")
	float TimeBeforeExp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|WeaponParam")
	int32 MaxAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|ObjectClass")
	TSoftClassPtr<UThrowWeaponBase>WeaponObject;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|ObjectClass")
	TSubclassOf<ABaseThrowProjectile>ProjectileClass;
};

USTRUCT(BlueprintType)
struct FEquipWeaponData : public FBaseWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|WeaponParam")
	int32 MaxAmmoInStorage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|WeaponParam")
	int32 MaxAmmoInWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|WeaponParam")
	float SelectWeaponTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|WeaponParam")
	float ReloadTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|WeaponParam")
	bool bCanAutoFire;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|WeaponParam")
	float RangeOfUse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|WeaponClass")
	TSoftClassPtr<URangeWeaponObjectBase>WeaponObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|WeaponCategory")
	EWeaponCategory Category;
};


UCLASS()
class SPACEWAR_API UWeaponDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	
	USkeletalMesh* GetWeaponMesh(TAssetPtr<USkeletalMesh> ItemMesh) const;
	FEquipWeaponData GetEquipWeaponData(const FName& WeaponName) const;
	FBaseThrowData GetThrowData(const FName& WeaponName) const;
	
	URangeWeaponObjectBase* CreateWeaponObject(const FName& WeaponName, UObject* WorldContext, UObject* Outer);
	UThrowWeaponBase* CreateThrowWeaponObject(const FName& WeaponName, UObject* WorldContext, UObject* Outer);

private:

	UPROPERTY(EditAnywhere)
	TMap<FName, FEquipWeaponData> EquipWeaponData;

	UPROPERTY(EditAnywhere)
	TMap<FName, FBaseThrowData> ThrowWeaponData;
	
};
