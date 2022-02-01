// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/EquipableWeaponManager.h"
#include "Components/StaminaComponent.h"
#include "GameInstances/BaseGameInstance.h"
#include "SpaceWar/Interfaces/UpdateAmmoInterface.h"
#include "Interfaces/GetDamageCauserInfo.h"
#include "Components/JetpackComponent.h"
#include "Objects/Armor/Base/BaseArmorObject.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "SpaceWar/Interfaces/FirstAidInterface.h"
#include "SpaceWarCharacter.generated.h"

class UHealthComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerInitializationComplete);

USTRUCT(BlueprintType)
struct FSoundData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundCue* UseJetpackSound;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundCue* StartUseSuperStaminaSound;
};

UCLASS(config=Game)
class ASpaceWarCharacter : public ACharacter, public IGetDamageCauserInfo, public IFirstAidInterface, public IUpdateAmmoInterface
{
	GENERATED_BODY()

	UFUNCTION()
	void CrouchPressed();

	UFUNCTION()
	void CrouchReleased();

	UFUNCTION()
	void JumpPressed();
	
	UFUNCTION()
	void JumpReleased();

	UFUNCTION()
	void UpdateWeaponMesh(UBaseWeaponObject* Weapon);

	UFUNCTION()
	void OnUpdateRecoilTimeLine(const FVector& Vector);

	UFUNCTION()
	void OnStaminaUsedEvent(bool bState);
	
	UFUNCTION()
	void CharDead();

	UFUNCTION(Server, Unreliable)
	void Server_UseJetpack();

	UFUNCTION(Server, Reliable)
	void Server_CreateWeapon(EWeaponType Type, const FName& Id);

	UFUNCTION(NetMulticast, Unreliable)
	void NetMulticast_PlayUseJetpackSound();

	UFUNCTION()
	void SyncLoadMesh(TAssetPtr<USkeletalMesh> MeshPtr);

	UFUNCTION(Server, Unreliable)
	void Server_StartUseAccessionWeapon();

	UFUNCTION()
	void ReplicateUpPitch();

	UFUNCTION(Server, Unreliable)
	void Server_StopUseAccessionWeapon();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_InitArmor(const FName& ArmorName);

/** call on client interface IUpdateAmmoInterface::RefreshAmmo */
	UFUNCTION(Server, Unreliable)
	void UpdateAmmo();

	void CreateArmor(const FName& Id);
	void ChangeFireModePressed();

	UFUNCTION(Server, Reliable)
	void Server_StartUseSuperSprint();

	UFUNCTION(Server, Reliable)
	void Server_StopUseSuperSprint();

	UFUNCTION(Server, Reliable)
	void Server_CreateThrow(const FName& Id);

	UFUNCTION()
	void ToggleUseSuperSprint();
	
	UFUNCTION()
	void OnSuperStaminaUsedEvent(bool bUse);

	UAudioComponent* SpawnSoundAttachedCue(USceneComponent* AttachComponent, USoundCue* Sound);

public:
	ASpaceWarCharacter();
	
	FVector GetCurrentFireTrace() const;
	void StopRecoilTimeline() { RecoilTimeline.Stop(); }
	void UpdateWeaponRecoil();
	
	virtual void GetCauserInfo_Implementation(FDamageCauserInfo& DamageCauserInfo) override;
	virtual void RefreshAmmo_Implementation() override;
	virtual void Jump() override;
	
	virtual void OnConstruction(const FTransform& Transform) override;

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE class UCameraComponent* GetAimCamera() const { return AimCamera; }

	FORCEINLINE USkeletalMeshComponent* GetLocalMesh() const;
	FORCEINLINE float GetLookUpPitch() const { return LookUpPitch; }

	UFUNCTION(BlueprintPure)
	UEquipableWeaponManager* GetWeaponManager() const { return WeaponManager; }
	
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

	UFUNCTION(Server, Reliable)
	void Server_ReplacementArmor(const FName& Id);
	
	UFUNCTION(Server, Reliable)
	void Server_ReplacementThrow(const FName& Id);
	
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
    
    UFUNCTION(BlueprintImplementableEvent)
	void CreateMinMapIcon(FName RowName);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateMeshMaterialByOwnerTeam();

public:

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(BlueprintAssignable, Category = "Player")
	FPlayerInitializationComplete OnPlayerInitializationComplete;

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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USphereComponent* RotateArmComponent;

private:

	UPROPERTY()
	UAIPerceptionStimuliSourceComponent* PerceptionStimuliSourceComponent;

	UPROPERTY()
	FTimeline RecoilTimeline;

	UPROPERTY(Replicated)
	float LookUpPitch;
	
	UPROPERTY(Replicated)
	bool bCanWeaponManipulation;

	UPROPERTY(Replicated)
	UBaseArmorObject* ArmorObject;
	
	UPROPERTY(EditAnywhere)
	UArmorDataAsset* ArmorDataAsset;

	UPROPERTY(EditAnywhere)
	FSoundData SoundData;

	/** update on "UpdateWeaponRecoil". Use for update control pitch and yaw when weapon used */
	FVector WeaponRecoil;
	
	bool bMoveForward;

	FTimerHandle FirstAidHandle;
};

