// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeaponObject.h"
#include "SpaceWar/Interfaces/CurrentCharacterObjectInterface.h"
#include "RangeWeaponObjectBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReload, bool, NewReloadState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAccessoryUsed, bool, NewState);

UCLASS(Blueprintable)
class SPACEWAR_API URangeWeaponObjectBase : public UBaseWeaponObject, public ICurrentCharacterObjectInterface
{
	GENERATED_BODY()

	void DropLineTrace(FHitResult& OutHit);
	void ApplyPointDamage(const FHitResult& Hit);
	FVector GetShootDirection();

	void ReloadStart();
	void ReloadStop();
	void PlayUseWeaponEffects();
	FVector FindRotateAround();
	
	UFUNCTION()
	void OnRep_Reload();

	UFUNCTION()
	void OnRep_AccessoryUse();

	UFUNCTION(Server, Unreliable)
	void Server_ReloadStart();

public:

	URangeWeaponObjectBase();

	virtual void Init(const FEquipWeaponData& NewData);

	virtual TAssetPtr<USkeletalMesh> GetWeaponMesh() override { return WeaponData.ItemMesh; }

	virtual bool UseCurrentPlayerObject_Implementation() override;
	virtual void UnUseCurrentPlayerObject_Implementation() override;

	virtual void StartAdditionalUsed();
	virtual void StopAdditionalUsed();
	bool OwnerStartAdditionalUsed();
	bool OwnerStopAdditionalUsed();
	
	UFUNCTION(BlueprintPure)
	int32 GetCurrentAmmo() const { return CurrentAmmoInWeapon; }
	
	UFUNCTION(BlueprintPure)
	int32 GetCurrentAmmoInStorage() const { return CurrentAmmoInStorage; }
	
	UFUNCTION(BlueprintPure)
	float GetAdditionalUse() const { return bAccessoryUsed; } 

	UFUNCTION(BlueprintCallable, Category = "Weapon|Button")
	void OwnerReload();

	UFUNCTION(BlueprintPure, Category = "Weapon|Getting")
	FEquipWeaponData GetWeaponData() const { return WeaponData; }
	
	/** Stop reload with field (with out calculate ammo) */
	UFUNCTION()
    void ClearReload();
    
	virtual void StopUseWeapon() override;

protected:

	virtual bool UseWeapon() override;
	virtual bool IsAbleToUseWeapon() override;
	virtual bool IsAbleToReload();

	virtual void OnWeaponSelectingEvent(bool NewState) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;

	/** Clear weapon timer used */
	virtual void StopRateDelay();

private:

	UPROPERTY(Replicated)
	int32 CurrentAmmoInWeapon;

	UPROPERTY(Replicated)
	int32 CurrentAmmoInStorage;

	UPROPERTY(ReplicatedUsing = OnRep_AccessoryUse)
	bool bAccessoryUsed;

	UPROPERTY(ReplicatedUsing = OnRep_Reload)
	bool bReloading;
	
	FTimerHandle ReloadHandle;

protected:
		
	UPROPERTY(Replicated)
	FEquipWeaponData WeaponData;

	UPROPERTY()
	float CurrentSpread;
	
public:

	UPROPERTY(BlueprintAssignable, Category = "Weapon|Delegate")
	FReload OnReload;

	UPROPERTY(BlueprintAssignable, Category = "Weapon|Delegate")
	FAccessoryUsed OnAccessoryUsed;
};
