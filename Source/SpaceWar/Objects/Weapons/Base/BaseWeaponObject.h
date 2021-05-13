// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/NetDriver.h"
#include "SpaceWar/DataAssets/WeaponDataAsset.h"
#include "BaseWeaponObject.generated.h"

class ASpaceWarCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOwnerChanged);

UCLASS(Abstract, BlueprintType)
class SPACEWAR_API UBaseWeaponObject : public UObject
{
	GENERATED_BODY()
	
public:

	UBaseWeaponObject();

	void SetCharacterOwner(ASpaceWarCharacter* NewOwner);
	bool GetWeaponUsed() const { return bWeaponUsed; }
	
	UFUNCTION(BlueprintCallable, Category = "Weapon|Pressed")
	virtual void OwnerStartUseWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapon|Pressed")
	virtual void OwnerStopUseWeapon();
	
	virtual TAssetPtr<USkeletalMesh> GetWeaponMesh() { return nullptr; }
	
	virtual void StopUseWeapon();
	
protected:

	UFUNCTION()
	virtual void OnRep_CharOwner();

	UFUNCTION()
	virtual void OnWeaponSelectingEvent(bool NewState);
	
	UFUNCTION(Server, Unreliable)
	virtual void Server_StartUseWeapon();
	
	UFUNCTION(Server, Unreliable)
    virtual void Server_StopUseWeapon();

	UFUNCTION(NetMulticast, Unreliable)
    virtual void NetMulticast_StartUseWeapon();

	UFUNCTION(NetMulticast, Unreliable)
    virtual void NetMulticast_StopUseWeapon();

	virtual void PostInitProperties() override;
	virtual void BeginPlay();
	
	/** define that object in supported network and cun be replicated */
    virtual bool IsSupportedForNetworking() const override { return true; }

	bool IsAuthority() const { return (GetWorld() == nullptr) || (GetWorld()->GetNetMode() != NM_Client) || GetWorld()->IsPlayingReplay(); }

	/** Net fun helpers */
	virtual int32 GetFunctionCallspace(UFunction* Function, FFrame* Stack) override;
	virtual bool CallRemoteFunction(UFunction* Function, void* Parms, FOutParmRec* OutParms, FFrame* Stack) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual bool UseWeapon();

	virtual bool IsAbleToUseWeapon();

public:

	UPROPERTY(BlueprintAssignable, Category = "Weapon|Delegates")
	FOwnerChanged OnOwnerChanged;
	
protected:

	UPROPERTY(ReplicatedUsing = OnRep_CharOwner)
	ASpaceWarCharacter* CharacterOwner;

	UPROPERTY(Replicated);
	bool bWeaponUsed;

	FTimerHandle UseWeaponHandle;
};
