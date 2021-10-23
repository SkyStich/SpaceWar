// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/NetDriver.h"
#include "SpaceWar/DataAssets/WeaponDataAsset.h"
#include "BaseWeaponObject.generated.h"

class ASpaceWarCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOwnerChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponUsed, bool, bIsUsed);

UCLASS(Abstract, BlueprintType)
class SPACEWAR_API UBaseWeaponObject : public UObject
{
	GENERATED_BODY()
	
public:

	UBaseWeaponObject();

	void SetCharacterOwner(ASpaceWarCharacter* NewOwner);
	
	UFUNCTION(BlueprintPure)
	bool GetWeaponUsed() const { return bWeaponUsed; }
	
	UFUNCTION(BlueprintCallable, Category = "Weapon|Pressed")
	virtual void OwnerStartUseWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapon|Pressed")
	virtual void OwnerStopUseWeapon();

	UFUNCTION(BlueprintPure, Category = "Weapon|Getting")
	FName GetWeaponName() const { return WeaponName; }

	void SetWeaponName(const FName& Name) { WeaponName = Name; } 
	
	virtual TAssetPtr<USkeletalMesh> GetWeaponMesh() { return nullptr; }
	virtual void StopUseWeapon();
	
	virtual bool GetAdditionalUse() const { return false; }
	virtual void StartAdditionalUsed() {}
	virtual void StopAdditionalUsed() {}
	virtual bool OwnerStartAdditionalUsed() { return false; }
	virtual bool OwnerStopAdditionalUsed() { return false; }
	
	/** Play sound at location */
	virtual void PlaySoundByCue(USoundCue* Sound, const FVector& Location, const FRotator& Rotation) {}

	/** play sound attached */
	virtual void PlaySoundByCue(USoundCue* Sound, USkeletalMeshComponent* AttachComponent, const FName& SocketName) {}

	virtual void PlaySound2DByCue(USoundCue* Sound){}

	UFUNCTION(BlueprintPure, Category = "Weapon|Getting")
	virtual float GetSelectTime() const { return 0.f; }

	UFUNCTION(BlueprintPure, Category = "Weapon|Getting")
	virtual UCurveVector* GetRecoilCurveVector() const { return nullptr; }

	UFUNCTION(BlueprintPure, Category = "Weapon|Getting")
	virtual float GetDelayBeforeUsed() const { return 0.f; }

	UFUNCTION(BlueprintPure, Category = "Weapon|Getting")
	virtual float GetReloadTime() const { return 0.f; }

	UFUNCTION(BlueprintPure, Category = "Weapon|Getting")
	virtual bool IsCanAutoFire() const { return false; }

	UFUNCTION(BlueprintPure, Category = "Weapon|Getting")
	virtual EWeaponCategory GetWeaponCategory() const { return EWeaponCategory::Special; }

	UFUNCTION(BlueprintPure, Category = "Weapon|Getting")
	virtual FString GetAmmoStatus() const { return "";}

	UFUNCTION(BlueprintPure, Category = "Weapon|Getting")
	virtual UAnimSequenceBase* GetReloadAnim() const { return nullptr; }	
	
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

	UPROPERTY(BlueprintAssignable, Category = "Weapon|Delegates")
	FWeaponUsed OnWeaponUsed;
	
protected:

	UPROPERTY(ReplicatedUsing = OnRep_CharOwner)
	ASpaceWarCharacter* CharacterOwner;

	UPROPERTY(Replicated);
	bool bWeaponUsed;
	
	UPROPERTY(Replicated);
	FName WeaponName;

	FTimerHandle UseWeaponHandle;
};
