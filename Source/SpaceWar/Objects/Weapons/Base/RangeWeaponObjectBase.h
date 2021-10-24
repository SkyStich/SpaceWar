// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeaponObject.h"
#include "Components/TimelineComponent.h"
#include "SpaceWar/Interfaces/CurrentCharacterObjectInterface.h"
#include "RangeWeaponObjectBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReload, bool, NewReloadState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAccessoryUsed, bool, NewState);

UCLASS(Blueprintable)
class SPACEWAR_API URangeWeaponObjectBase : public UBaseWeaponObject, public ICurrentCharacterObjectInterface
{
	GENERATED_BODY()
	
	void ApplyPointDamage(const FHitResult& Hit);
	void ReloadStart();
	void ReloadStop();
	void PlayUseWeaponEffects();

	virtual void PlaySoundByCue(USoundCue* Sound, const FVector& Location, const FRotator& Rotation) override;
	virtual void PlaySoundByCue(USoundCue* Sound, USkeletalMeshComponent* AttachComponent, const FName& SocketName) override;
	virtual void PlaySound2DByCue(USoundCue* Sound) override;
	
	FVector FindRotateAround();
	FVector GetShootDirection();
	
	UFUNCTION()
	void OnRep_Reload();

	UFUNCTION()
	void OnRep_AccessoryUse();

	UFUNCTION(Server, Unreliable)
	void Server_ReloadStart();

public:

	URangeWeaponObjectBase();

	virtual void Init(const FEquipWeaponData& NewData);
	void AddAmmo(int32 const Amount);

	virtual TAssetPtr<USkeletalMesh> GetWeaponMesh() override { return WeaponData.ItemMesh; }

	virtual bool UseCurrentPlayerObject_Implementation() override;
	virtual void UnUseCurrentPlayerObject_Implementation() override;
	virtual void StartAdditionalUsed() override;
	virtual void StopAdditionalUsed() override;
	virtual bool OwnerStartAdditionalUsed() override;
	virtual bool OwnerStopAdditionalUsed() override;
	virtual bool OwnerStartUseWeapon() override;

	virtual float GetSelectTime() const override { return WeaponData.WeaponCharacteristicsBase.SelectWeaponTime; }
	virtual float GetReloadTime() const override { return WeaponData.RangeWeaponCharacteristics.ReloadTime; }
	virtual float GetDelayBeforeUsed() const override { return WeaponData.WeaponCharacteristicsBase.DelayBeforeUse; }
	virtual bool IsCanAutoFire() const override { return WeaponData.RangeWeaponCharacteristics.bCanAutoFire; }
	virtual EWeaponCategory GetWeaponCategory() const override { return WeaponData.WeaponCharacteristicsBase.Category; }
	virtual UAnimSequenceBase* GetReloadAnim() const override { return WeaponData.Animations.ReloadAnim; }
	virtual UCurveVector* GetRecoilCurveVector() const override { return WeaponData.Spreads.RecoilCurve; }
	virtual FString GetAmmoStatus() const override;
	
	UFUNCTION(BlueprintPure)
	int32 GetCurrentAmmo() const { return CurrentAmmoInWeapon; }
	
	UFUNCTION(BlueprintPure)
	int32 GetCurrentAmmoInStorage() const { return CurrentAmmoInStorage; }
	
	UFUNCTION(BlueprintPure)
	virtual bool GetAdditionalUse() const override { return bAccessoryUsed; } 

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
	virtual void DropLineTrace();

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
