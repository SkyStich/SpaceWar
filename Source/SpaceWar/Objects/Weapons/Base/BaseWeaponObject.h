// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/NetDriver.h"
#include "SpaceWar/DataAssets/WeaponDataAsset.h"
#include "BaseWeaponObject.generated.h"

class ASpaceWarCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOwnerChanged);

UCLASS()
class SPACEWAR_API UBaseWeaponObject : public UObject
{
	GENERATED_BODY()
	
public:

	UBaseWeaponObject();

	void SetCharacterOwner(ASpaceWarCharacter* NewOwner);
	void SetWeaponData(const FEquipWeaponData& NewData) { WeaponData = NewData; }

	TAssetPtr<USkeletalMesh> GetWeaponMesh() { return WeaponData.ItemMesh; }

protected:

	UFUNCTION()
	virtual void OnRep_CharOwner();

	virtual void PostInitProperties() override;
	virtual void BeginPlay();
	
	/** define that object in supported network and cun be replicated */
    virtual bool IsSupportedForNetworking() const override { return true; }

	bool IsAuthority() const { return (GetWorld() == nullptr) || (GetWorld()->GetNetMode() != NM_Client) || GetWorld()->IsPlayingReplay(); }

	/** Net fun helpers */
	virtual int32 GetFunctionCallspace(UFunction* Function, FFrame* Stack) override;
	virtual bool CallRemoteFunction(UFunction* Function, void* Parms, FOutParmRec* OutParms, FFrame* Stack) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:

	UPROPERTY(BlueprintAssignable, Category = "Weapon|Delegates")
	FOwnerChanged OnOwnerChanged;
	
protected:

	UPROPERTY(ReplicatedUsing = OnRep_CharOwner)
	ASpaceWarCharacter* CharacterOwner;
	
	UPROPERTY(Replicated)
	FEquipWeaponData WeaponData;
};
