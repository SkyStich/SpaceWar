// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../DataAssets/WeaponDataAsset.h"
#include "SpaceWar/Objects/Weapons/Base/RangeWeaponObjectBase.h"
#include "SpaceWar/Objects/WeaponsThrow/ThrowWeaponBase.h"

#include "EquipableWeaponManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCurrentWeaponChanged, URangeWeaponObjectBase*, NewCurrentWeapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponSelect, bool, NewState);

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	FirstWeapon,
	SecondWeapon
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEWAR_API UEquipableWeaponManager : public UActorComponent
{
	GENERATED_BODY()

	UFUNCTION()
	void OnRep_CurrentWeapon();

	UFUNCTION()
	void OnRep_WeaponSelect();
	
	void AddWeaponToStorage(EWeaponType Key, URangeWeaponObjectBase* Value);

	void SelectWeapon(EWeaponType NewType);
	void FinishWeaponSelect();

	UFUNCTION(Server, Unreliable)
	void Server_SelectWeapon(EWeaponType NewType);
	
	UFUNCTION()
	void OnRep_ThrowWeapon();

	UFUNCTION()
	void OnThrowUsed(bool bUsed);
	
	UFUNCTION()
	void CurrentWeaponUnEquip(URangeWeaponObjectBase* NewWeapon);
	
public:	

	UEquipableWeaponManager();

	UFUNCTION(BlueprintPure, Category = "Weapons")
	TMap<EWeaponType, URangeWeaponObjectBase*> GetWeapons() const { return Weapons; }

	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable)
	bool CreateThrow(const FName Name);
        
	UFUNCTION(BlueprintCallable, Category = "WeaponManager|Create")
	URangeWeaponObjectBase* CreateWeaponByName(const FName& Name, EWeaponType Type);

	UFUNCTION(BlueprintCallable, Category = "WeaponManager|Server")
	void SetCurrentWeapon(URangeWeaponObjectBase* NewWeapon);

	UFUNCTION(BlueprintCallable, Category = "WeaponManager|Button")
	void OwnerWeaponSelect(EWeaponType NewType);

	UFUNCTION(BlueprintPure, Category = "Weapons|Getting")
	URangeWeaponObjectBase* GetCurrentWeapon() const { return CurrentWeapon; } 
	
	UFUNCTION(BlueprintPure, Category = "Weapons|Getting")
	UThrowWeaponBase* GetThrow() const { return ThrowWeaponBase; }
	
	UFUNCTION(BlueprintPure, Category = "Weapons|Getting")
	bool GetWeaponSelect() const { return bWeaponSelect; }

	UFUNCTION(BlueprintPure, Category = "Weapons|Getting")
	bool GetThrowUsed() const { return ThrowWeaponBase->GetWeaponUsed(); }
	
protected:

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

private:

	/** Set in blueprints */
	UPROPERTY(EditAnywhere, Category = "DataAsset")
	UWeaponDataAsset* WeaponDataAsset;

	UPROPERTY(Replicated)
	TMap<EWeaponType, URangeWeaponObjectBase*> Weapons;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentWeapon)
	URangeWeaponObjectBase* CurrentWeapon;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentWeapon)
	URangeWeaponObjectBase* OldWeapon;
	
	UPROPERTY(ReplicatedUsing = OnRep_ThrowWeapon)
	class UThrowWeaponBase* ThrowWeaponBase;

	UPROPERTY(ReplicatedUsing = OnRep_WeaponSelect)
	bool bWeaponSelect;

	FTimerHandle SelectWeaponHandle;

public:

	UPROPERTY(BlueprintAssignable, Category = "WeaponManager|Delegate")
	FCurrentWeaponChanged OnCurrentWeaponChanged;

	UPROPERTY(BlueprintAssignable, Category = "WeaponManager|Delegate")
	FWeaponSelect OnWeaponSelect;
};
