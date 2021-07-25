// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../DataAssets/WeaponDataAsset.h"
#include "SpaceWar/Objects/Weapons/Base/RangeWeaponObjectBase.h"
#include "SpaceWar/Objects/WeaponsThrow/ThrowWeaponBase.h"
#include "EquipableWeaponManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCurrentWeaponChanged, UBaseWeaponObject*, NewCurrentWeapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponSelect, bool, NewState);

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Unknown,
	FirstWeapon,
	SecondWeapon,
	SpecialWeapon
};

USTRUCT(BlueprintType)
struct FWeapons
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	EWeaponType Key;

	UPROPERTY(BlueprintReadOnly)
	UBaseWeaponObject* Value;

	FWeapons() : Key(EWeaponType::FirstWeapon), Value(nullptr) {}
	FWeapons(EWeaponType NewKey, UBaseWeaponObject* NewValue) : Key(NewKey), Value(NewValue) {}
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
	void FinishWeaponSelect();

	UFUNCTION(Server, Unreliable)
	void Server_SelectWeapon(EWeaponType NewType);
	
	UFUNCTION()
	void OnRep_ThrowWeapon();

	UFUNCTION()
	void OnThrowUsed(bool bUsed);
	
	UFUNCTION()
	void CurrentWeaponUnEquip(UBaseWeaponObject* NewWeapon);

	void AddToWeapons(EWeaponType Type, UBaseWeaponObject* Value);
	void RemoveFromWeapons(EWeaponType Type);
	UBaseWeaponObject* FindFromWeapon(EWeaponType Type);
	
public:	

	UEquipableWeaponManager();

	UFUNCTION(BlueprintPure, Category = "Weapons")
	TArray<FWeapons> GetWeapons() const { return Weapons; }

	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable)
	bool CreateThrow(const FName Name);
        
	UFUNCTION(BlueprintCallable, Category = "WeaponManager|Create")
	URangeWeaponObjectBase* CreateWeaponByName(const FName& Name, EWeaponType Type);

	UFUNCTION(BlueprintCallable, Category = "WeaponManager|Server")
	void SetCurrentWeapon(UBaseWeaponObject* NewWeapon);

	UFUNCTION(BlueprintCallable, Category = "WeaponManager|Button")
	void OwnerWeaponSelect(EWeaponType NewType);
	
	UFUNCTION(BlueprintCallable, Category = "WeaponManager|Weapon")
	void WeaponReplacement(EWeaponType NewType, const FName& Id);

	UFUNCTION(BlueprintPure, Category = "Weapons|Getting")
	UBaseWeaponObject* GetCurrentWeapon() const { return CurrentWeapon; } 
	
	UFUNCTION(BlueprintPure, Category = "Weapons|Getting")
	UThrowWeaponBase* GetThrow() const { return ThrowWeaponBase; }
	
	UFUNCTION(BlueprintPure, Category = "Weapons|Getting")
	bool GetWeaponSelect() const { return bWeaponSelect; }

	UFUNCTION(BlueprintPure, Category = "Weapons|Getting")
	bool GetThrowUsed() const { return ThrowWeaponBase->GetWeaponUsed(); }

	UFUNCTION(BlueprintCallable, Server, Unreliable, Category = "WeaponManager|Weapon")
	void Server_ReplacementWeapon(EWeaponType Key, const FName& Id);
	
protected:

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

private:

	/** Set in blueprints */
	UPROPERTY(EditAnywhere, Category = "DataAsset")
	UWeaponDataAsset* WeaponDataAsset;

	UPROPERTY(Replicated)
	TArray<FWeapons> Weapons;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentWeapon)
	UBaseWeaponObject* CurrentWeapon;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentWeapon)
	UBaseWeaponObject* OldWeapon;
	
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
