// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/EquipableWeaponManager.h"
#include "Interfaces/GetDamageCauserInfo.h"
#include "SpaceWarCharacter.generated.h"

UCLASS(config=Game)
class ASpaceWarCharacter : public ACharacter, public IGetDamageCauserInfo
{
	GENERATED_BODY()

	UFUNCTION()
	void UpdateWeaponMesh(UBaseWeaponObject* Weapon);

public:
	ASpaceWarCharacter();

	void GetCauserInfo_Implementation(FDamageCauserInfo& DamageCauserInfo) override;

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE USkeletalMeshComponent* GetLocalMesh() const;
	FORCEINLINE float GetLookUpPitch() const { return LookUpPitch; }
	FORCEINLINE UEquipableWeaponManager* GetWeaponManager() const { return WeaponManager; }

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	
	void MoveForward(float Value);
	void MoveRight(float Value);

	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SkeletalMesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SkeletalArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SkeletalMesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SkeletalMesh, meta = (AllowPrivateAccess = "true"))
	UEquipableWeaponManager* WeaponManager;

private:

	UPROPERTY(Replicated)
	float LookUpPitch;
};

