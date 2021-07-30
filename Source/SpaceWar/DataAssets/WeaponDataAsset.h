// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Sound/SoundCue.h"
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

	/** display weapon name. Not Id */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|Description")
	FName WeaponName;

	/** delay before weapon use */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|WeaponParam")
	float DelayBeforeUse;

	/** time for select weapon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|WeaponParam")
	float SelectWeaponTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|WeaponCategory")
	EWeaponCategory Category;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|Description")
	TAssetPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|Base")
	TAssetPtr<USkeletalMesh> ItemMesh;
};

USTRUCT()
struct FBaseThrowData : public FBaseWeaponData
{
	GENERATED_BODY()

	/** Exp range  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|WeaponParam")
	float ExpRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|WeaponParam")
	float BaseDamage;

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
	float BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|WeaponParam")
	float ReloadTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|WeaponParam")
	int32 MaxDamageDistance;  // Write in cm 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|WeaponParam")
	bool bCanAutoFire;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|WeaponParam")
	float RangeOfUse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|WeaponParam")
	FRandomStream FireRandomStream;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|WeaponParam")
	float MinSpread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|WeaponParam")
	float MaxSpread;

	/** with 0 to 10 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|WeaponParam")
	int32 AccuracyInSight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|WeaponClass")
	TSoftClassPtr<URangeWeaponObjectBase>WeaponObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|ClientEffects")
	UParticleSystem* MuzzleParticle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDataAsset|WeaponParam")
	USoundBase* MuzzleSound;
};


UCLASS(BlueprintType)
class SPACEWAR_API UWeaponDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	
	USkeletalMesh* GetWeaponMesh(TAssetPtr<USkeletalMesh> ItemMesh) const;
	FEquipWeaponData GetEquipWeaponData(const FName& WeaponName) const;
	FBaseThrowData GetThrowData(const FName& WeaponName) const;
	
	URangeWeaponObjectBase* CreateWeaponObject(const FName& WeaponName, UObject* WorldContext, UObject* Outer);
	UThrowWeaponBase* CreateThrowWeaponObject(const FName& WeaponName, UObject* WorldContext, UObject* Outer);

	UFUNCTION(BlueprintCallable)
	UTexture2D* GetIcon(TAssetPtr<UTexture2D> IconPtr);
	
	UFUNCTION(BlueprintPure, Category = "DataAsset")
	TMap<FName, FEquipWeaponData> GetWeaponData() const { return EquipWeaponData; }
	
	UFUNCTION(BlueprintPure, Category = "DataAsset")
	TMap<FName, FBaseThrowData> GetThrowData() const { return ThrowWeaponData; }
	

private:

	UPROPERTY(EditAnywhere)
	TMap<FName, FEquipWeaponData> EquipWeaponData;

	UPROPERTY(EditAnywhere)
	TMap<FName, FBaseThrowData> ThrowWeaponData;
	
};
