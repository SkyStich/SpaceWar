// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RemoveServerCallBack.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FRemoveServerDelegate, bool, bResult, const FString&, ErrorMessage);

USTRUCT()
struct FRemoveServerCallBack
{
	GENERATED_BODY()

	FRemoveServerCallBack() : OnRemoveServerDelegate(FRemoveServerDelegate()) {}
	
	FRemoveServerDelegate OnRemoveServerDelegate;
};