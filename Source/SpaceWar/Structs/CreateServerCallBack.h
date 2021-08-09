// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CreateServerCallBack.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FCreateServerDelegate, bool, bResult, const FString&, ErrorMessage);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FGameServerAddress, const FString&, Address, const FString&, ErrorMessage);

USTRUCT(BlueprintType)
struct FCreateServerCallBackInDataBase
{
	GENERATED_BODY()
	
	FCreateServerCallBackInDataBase() : OnCreateServerDelegate(FCreateServerDelegate()) {}
	FCreateServerCallBackInDataBase(const FCreateServerDelegate& Delegate) : OnCreateServerDelegate(Delegate) {}

	UPROPERTY()
	FCreateServerDelegate OnCreateServerDelegate;
};

USTRUCT(BlueprintType)
struct FGameAddressCallBack
{
	GENERATED_BODY()
	
	FGameAddressCallBack() : OnGameServerAddress(FCreateServerDelegate()) {}
	FGameAddressCallBack(const FCreateServerDelegate& Delegate) : OnGameServerAddress(Delegate) {}

	UPROPERTY()
	FGameServerAddress OnGameServerAddress;
};