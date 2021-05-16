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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|Base")
	FName WeaponName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|Base")
	float BaseDamage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|Base")
	float DelayBeforeUse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|Base")
	TAssetPtr<USkeletalMesh> ItemMesh;
};

USTRUCT()
struct FBaseThrowData : public FBaseWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|Base")
	float ExpRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|Base")
	float TimeBeforeExp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|Base")
	int32 MaxAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|Equipable")
	TSoftClassPtr<UThrowWeaponBase>WeaponObject;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|Equipable")
	TSubclassOf<ABaseThrowProjectile>ProjectileClass;
};

USTRUCT(BlueprintType)
struct FEquipWeaponData : public FBaseWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|Equipable")
	int32 MaxAmmoInStorage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|Equipable")
	int32 MaxAmmoInWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|Base")
	float SelectWeaponTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|Equipable")
	float ReloadTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|Equipable")
	bool bCanAutoFire;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|Base")
	float RangeOfUse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|Equipable")
	TSoftClassPtr<URangeWeaponObjectBase>WeaponObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|Equipable")
	EWeaponCategory Category;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|Equipable")
	UAnimSequence* ReloadInstance;
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
