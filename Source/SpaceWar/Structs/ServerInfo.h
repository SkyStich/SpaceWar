// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ServerInfo.generated.h"

/** use for server */
USTRUCT(BlueprintType)
struct FServersData
{
	GENERATED_BODY()

	FServersData() : Id(-1), Name(""), Address("") {}
	
	UPROPERTY(BlueprintReadOnly)
	int32 Id;
	
	UPROPERTY(BlueprintReadOnly)
	FString Name;

	UPROPERTY(BlueprintReadOnly)
	FString Address;
};

DECLARE_DYNAMIC_DELEGATE_ThreeParams(FGetServerInfoDelegate, bool, bResult, const FString&, ErrorMessage, const FServersData&, ServerData);

USTRUCT(BlueprintType)
struct FGetServerInfoCallBack
{
	GENERATED_BODY()

	FGetServerInfoCallBack() : OnGetServerInfoDelegate(FGetServerInfoDelegate()) {}
	FGetServerInfoCallBack(const FGetServerInfoDelegate& CallBack) : OnGetServerInfoDelegate(CallBack) {}
	
	FGetServerInfoDelegate OnGetServerInfoDelegate;
};

