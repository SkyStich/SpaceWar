// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeaponObject.h"
#include "RangeWeaponObjectBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReload, bool, NewReloadState);

UCLASS(Blueprintable)
class SPACEWAR_API URangeWeaponObjectBase : public UBaseWeaponObject
{
	GENERATED_BODY()

	void DropLineTrace(FHitResult& OutHit);
	void ApplyPointDamage();
	FVector GetShootDirection();

	void ReloadStart();
	void ReloadStop();

	/** Stop reload with field (with out calculate ammo) */
	UFUNCTION()
	void ClearReload();

	UFUNCTION()
	void OnRep_Reload();

	UFUNCTION(Server, Unreliable)
	void Server_ReloadStart();

public:

	URangeWeaponObjectBase();

	virtual void Init(const FEquipWeaponData& NewData) override;
	
	UFUNCTION(BlueprintPure)
	int32 GetCurrentAmmo() const { return CurrentAmmoInWeapon; }
	
	UFUNCTION(BlueprintPure)
	int32 GetCurrentAmmoInStorage() const { return CurrentAmmoInStorage; }

	UFUNCTION(BlueprintCallable, Category = "Weapon|Button")
	void OwnerReload();

protected:

	virtual bool UseWeapon() override;
	virtual void StopUseWeapon() override;
	virtual bool IsAbleToUseWeapon() override;
	virtual bool IsAbleToReload();

	virtual void OnWeaponSelectingEvent(bool NewState) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;

private:

	UPROPERTY(Replicated)
	int32 CurrentAmmoInWeapon;

	UPROPERTY(Replicated)
	int32 CurrentAmmoInStorage;

	UPROPERTY(ReplicatedUsing = OnRep_Reload)
	bool bReloading;
	
	FTimerHandle ReloadHandle;

public:

	UPROPERTY(BlueprintAssignable, Category = "Weapon|Delegate")
	FReload OnReload;
};
