// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/EquipableWeaponManager.h"
#include "Components/StaminaComponent.h"
#include "SpaceWar/Interfaces/UpdateAmmoInterface.h"
#include "Interfaces/GetDamageCauserInfo.h"
#include "Components/JetpackComponent.h"
#include "Objects/Armor/Base/BaseArmorObject.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "SpaceWar/Interfaces/FirstAidInterface.h"
#include "SpaceWarCharacter.generated.h"

class UHealthComponent;

UCLASS(config=Game)
class ASpaceWarCharacter : public ACharacter, public IGetDamageCauserInfo, public IFirstAidInterface, public IUpdateAmmoInterface
{
	GENERATED_BODY()

	UFUNCTION()
	void UpdateWeaponMesh(UBaseWeaponObject* Weapon);

	UFUNCTION()
	void OnStaminaUsedEvent(bool bState);
	
	UFUNCTION()
	void CharDead();

	UFUNCTION(Server, Unreliable)
	void Server_UseJetpack();

	UFUNCTION()
	void SyncLoadMesh(TAssetPtr<USkeletalMesh> MeshPtr);

	UFUNCTION(Server, Unreliable)
	void Server_StartUseAccessionWeapon();

	UFUNCTION()
	void ReplicateUpPitch();

	UFUNCTION(Server, Unreliable)
	void Server_StopUseAccessionWeapon();

	UFUNCTION(Server, Reliable, WithValidation)
	void InitArmor(const FName& ArmorName);

/** call on client interface IUpdateAmmoInterface::RefreshAmmo */
	UFUNCTION(Server, Unreliable)
	void UpdateAmmo();

public:
	ASpaceWarCharacter();
	
	FVector GetCurrentFireTrace() const;

	virtual void GetCauserInfo_Implementation(FDamageCauserInfo& DamageCauserInfo) override;
	virtual void RefreshAmmo_Implementation() override;
	
	virtual void OnConstruction(const FTransform& Transform) override;

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE USkeletalMeshComponent* GetLocalMesh() const;
	FORCEINLINE float GetLookUpPitch() const { return LookUpPitch; }
	FORCEINLINE UEquipableWeaponManager* GetWeaponManager() const { return WeaponManager; }
	
	UFUNCTION(BlueprintCallable, Category = "Character|Getting")
	USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; } 
	
	UFUNCTION(BlueprintPure, Category = "Character|Getting")
	FORCEINLINE UHealthComponent* GetHealthComponent() const { return HealthComponent; }
	
	UFUNCTION(BlueprintPure, Category = "Character|Getting")
	FORCEINLINE UStaminaComponent* GetStaminaComponent() const { return StaminaComponent; }

	UFUNCTION(BlueprintCallable, Category = "Character|Keyboard")
	void OwnerStartUseStamina();
	
	UFUNCTION(BlueprintCallable, Category = "Character|Keyboard")
	void OwnerStopUseStamina();

	UFUNCTION(BlueprintCallable, Category = "Character|Keyboard")
	void OwnerStartAdditionalUse();

	UFUNCTION(BlueprintCallable, Category = "Character|Keyboard")
	void OwnerStopAdditionalUse();

	UFUNCTION(BlueprintPure, Category = "Player|Weapon")
	bool IsCanWeaponManipulation() const { return bCanWeaponManipulation; }
	
	UFUNCTION()
	void SetCanWeaponManipulation(bool NewState) { bCanWeaponManipulation = NewState; }
	
	UFUNCTION()
	void StartUseWeapon();

	UFUNCTION()
	void StopUseWeapon();

protected:
	
	void MoveForward(float Value);
	void MoveRight(float Value);

	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	virtual void StartPlayerFirstAid_Implementation(ETeam Team, float const Value) override;
	virtual void StopPlayerFirstAid_Implementation() override;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;	
	
	UFUNCTION(BlueprintCallable)
	void UseJetpackPressed();
	
	UFUNCTION(BlueprintPure)
	UBaseArmorObject* GetArmorObject() const { return ArmorObject; }

	UFUNCTION(BlueprintImplementableEvent)
    void StartAiming();

	UFUNCTION(BlueprintImplementableEvent)
    void StopAiming();

public:

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:
	
	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* AimCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SkeletalMesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SkeletalArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SkeletalMesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SkeletalMesh, meta = (AllowPrivateAccess = "true"))
	UEquipableWeaponManager* WeaponManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SkeletalMesh, meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SkeletalMesh, meta = (AllowPrivateAccess = "true"))
	UJetpackComponent* JetpackComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SkeletalMesh, meta = (AllowPrivateAccess = "true"))
	UStaminaComponent* StaminaComponent;

private:

	UPROPERTY()
	UAIPerceptionStimuliSourceComponent* PerceptionStimuliSourceComponent;

	UPROPERTY(Replicated)
	float LookUpPitch;
	
	UPROPERTY(Replicated)
	bool bCanWeaponManipulation;

	UPROPERTY(Replicated)
	UBaseArmorObject* ArmorObject;
	
	UPROPERTY(EditAnywhere)
	UArmorDataAsset* ArmorDataAsset;
	
	bool bMoveForward;

	FTimerHandle FirstAidHandle;
};

