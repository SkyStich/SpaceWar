// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GetServerListCallBack.generated.h"

/** use for client */
USTRUCT(BlueprintType)
struct FClientServerInfo
{
	GENERATED_BODY();
	
	FClientServerInfo() : ServerName(""), Address("") {}
	FClientServerInfo(const FString& Name, const FString& ServerAddress) : ServerName(Name), Address(ServerAddress) {}

	UPROPERTY(BlueprintReadOnly)
	FString ServerName;
	
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