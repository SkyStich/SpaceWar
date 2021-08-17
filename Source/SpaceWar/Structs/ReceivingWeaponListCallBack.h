// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReceivingWeaponListCallBack.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FReceivingWeaponListDelegate, const TArray<FString>&, WaaponList);

USTRUCT(BlueprintType)
struct FReceivingWeaponListCallBack
{
	GENERATED_BODY()
	
	FReceivingWeaponListCallBack() : OnReceivingWeaponListDelegate(FReceivingWeaponListDelegate()) {}
	FReceivingWeaponListCallBack(const FReceivingWeaponListDelegate& CallBack) : OnReceivingWeaponListDelegate(CallBack) {}

	UPROPERTY()
	FReceivingWeaponListDelegate OnReceivingWeaponListDelegate;
};