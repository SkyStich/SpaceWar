// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RegisterUsersCallBack.generated.h"


DECLARE_DYNAMIC_DELEGATE_ThreeParams(FDelegateRequestRegisterUserCallBack, bool, bResult, const FString&, SessionKey, const FString&, ErrorMessage);

USTRUCT()
struct FCallBackRequestRegisterUser
{
	GENERATED_BODY()

	FCallBackRequestRegisterUser() : OnRequestRegisterUserCallBack(FDelegateRequestRegisterUserCallBack()) {}
	FCallBackRequestRegisterUser(const FDelegateRequestRegisterUserCallBack& CallBack) : OnRequestRegisterUserCallBack(CallBack) {}

	UPROPERTY()
	FDelegateRequestRegisterUserCallBack OnRequestRegisterUserCallBack;
};



