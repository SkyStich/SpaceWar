// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Objects/Weapons/Base/BaseWeaponObject.h"
#include "../DataAssets/WeaponDataAsset.h"
#include "EquipableWeaponManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCurrentWeaponChanged, UBaseWeaponObject*, NewCurrentWeapon);
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
	
	void AddWeaponToStorage(EWeaponType Key, UBaseWeaponObject* Value);

	void SelectWeapon(EWeaponType NewType);
	void FinishWeaponSelect(UBaseWeaponObject* NewWeapon);

	UFUNCTION(Server, Unreliable)
	void Server_SelectWeapon(EWeaponType NewType);

public:	

	UEquipableWeaponManager();

	UFUNCTION(BlueprintPure, Category = "Weapons")
	TMap<EWeaponType, UBaseWeaponObject*> GetWeapons() const { return Weapons; }

	UFUNCTION(BlueprintCallable, Category = "WeaponManager|Create")
	UBaseWeaponObject* CreateWeaponByName(const FName& Name, EWeaponType Type);

	UFUNCTION(BlueprintCallable, Category = "WeaponManager|Server")
	void SetCurrentWeapon(UBaseWeaponObject* NewWeapon);

	UFUNCTION(BlueprintCallable, Category = "WeaponManager|Button")
	void OwnerWeaponSelect(EWeaponType NewType);

	UFUNCTION(BlueprintPure, Category = "Weapons|Getting")
	UBaseWeaponObject* GetCurrentWeapon() const { return CurrentWeapon; } 
	
	UFUNCTION(BlueprintPure, Category = "Weapons|Getting")
	bool GetWeaponSelect() const { return bWeaponSelect; }
	
protected:

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

private:

	/** Set in blueprints */
	UPROPERTY(EditAnywhere, Category = "DataAsset")
	UWeaponDataAsset* WeaponDataAsset;

	UPROPERTY(Replicated)
	TMap<EWeaponType, UBaseWeaponObject*> Weapons;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentWeapon)
	UBaseWeaponObject* CurrentWeapon;

	UPROPERTY(ReplicatedUsing = OnRep_WeaponSelect)
	bool bWeaponSelect;

	FTimerHandle SelectWeaponHandle;

public:

	UPROPERTY(BlueprintAssignable, Category = "WeaponManager|Delegate")
	FCurrentWeaponChanged OnCurrentWeaponChanged;

	UPROPERTY(BlueprintAssignable, Category = "WeaponManager|Delegate")
	FWeaponSelect OnWeaponSelect;
};
