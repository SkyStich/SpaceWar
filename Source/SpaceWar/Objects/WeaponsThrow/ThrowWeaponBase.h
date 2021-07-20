// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpaceWar/Objects/Weapons/Base/BaseWeaponObject.h"
#include "UObject/NoExportTypes.h"
#include "ThrowWeaponBase.generated.h"

class ASpaceWarCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeaponThrow);

UCLASS(Abstract)
class SPACEWAR_API UThrowWeaponBase : public UBaseWeaponObject
{
	GENERATED_BODY()

protected:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual bool UseWeapon() override;
	virtual void StopUseWeapon() override;
	virtual bool IsAbleToUseWeapon() override;

	virtual void PreparationForUse() {}
	
public:

	UThrowWeaponBase();
	void Init(const FBaseThrowData& NewData);
	void IncrementCurrentAmount();

	FBaseThrowData GetThrowData() const { return ThrowData; }
	int32 GetCurrentAmount() const { return CurrentAmountThrow; }

protected:

	UPROPERTY(Replicated)
	FBaseThrowData ThrowData;

	UPROPERTY()
	FTimerHandle PreparationForUseHandle;
	
	UPROPERTY(Replicated)
	int32 CurrentAmountThrow;

public:

	UPROPERTY(BlueprintAssignable, Category = "Object|Throw")
	FWeaponThrow OnWeaponThrow;
};
