// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpaceWar/DataAssets/ArmorDataAsset.h"
#include "UObject/NoExportTypes.h"
#include "BaseArmorObject.generated.h"

class ASpaceWarCharacter;

UCLASS()
class SPACEWAR_API UBaseArmorObject : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure)
	FArmorData GetData() const { return ArmorData; }

protected:

	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual bool CallRemoteFunction(UFunction* Function, void* Parms, FOutParmRec* OutParms, FFrame* Stack) override;
	virtual int32 GetFunctionCallspace(UFunction* Function, FFrame* Stack) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:

	UPROPERTY(Replicated)
	FArmorData ArmorData;

friend UArmorDataAsset;
};
