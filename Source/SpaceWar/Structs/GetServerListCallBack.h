// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GetServerListCallBack.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FServerNameVerificationCallback, bool, bResult /** true if server passed the verification */);

/** use for client */
USTRUCT(BlueprintType)
struct FClientServerInfo
{
	GENERATED_BODY();
	
	FClientServerInfo() : ServerName(""), MapName(""), Address("") {}
	FClientServerInfo(const FString& Name, const FString& NewMapName, const FString& ServerAddress) : ServerName(Name), MapName(NewMapName), Address(ServerAddress) {}

	UPROPERTY(BlueprintReadOnly)
	FString ServerName;
	
	UPROPERTY(BlueprintReadOnly)
	FString MapName;
	
	UPROPERTY(BlueprintReadOnly)
	FString Address;
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FGetServerListDelegate, const TArray<FClientServerInfo>&, ServersList);

USTRUCT(BlueprintType)
struct FGetServerListCallBack
{
	GENERATED_BODY()

	FGetServerListCallBack() : OnGetServerListDelegate(FGetServerListDelegate()) {}
	
	FGetServerListDelegate OnGetServerListDelegate;
};

USTRUCT(BlueprintType)
struct FGetServerHUDListCallBack
{
	GENERATED_BODY()

	FGetServerHUDListCallBack() : ServerName(""), Address("") {}
	FGetServerHUDListCallBack(const FString& Name, const FString& ServerAddress) : ServerName(Name), Address(ServerAddress) {}

	UPROPERTY(BlueprintReadOnly)
	FString ServerName;
	
	UPROPERTY(BlueprintReadOnly)
	FString Address;
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FGetServerHudListDelegate, const TArray<FGetServerHUDListCallBack>&, Callback);