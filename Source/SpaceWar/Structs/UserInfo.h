// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserInfo.generated.h"

USTRUCT(BlueprintType)
struct FUserInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString Login;

	UPROPERTY(BlueprintReadWrite)
	FString Password;
};

USTRUCT(BlueprintType)
struct FRegisterInfo
{
	GENERATED_BODY()

	FRegisterInfo() : Login(""), PlayerEmail(""), Password(""), RepeatPass("") {}
	FRegisterInfo(const FString& Log, const FString& Email, const FString& Pass, const FString& RepeatPass) : Login(Log), PlayerEmail(Email), Password(Pass), RepeatPass(RepeatPass) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Login;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString PlayerEmail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Password;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString RepeatPass;
};

