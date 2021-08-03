// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserInfo.generated.h"

USTRUCT()
struct FUserInfo
{
	GENERATED_BODY()

	FUserInfo() : Login(""), Password("") {}
	FUserInfo(const FString& Log, const FString& Pass) : Login(Log), Password(Pass) {}

	FString GetPass() const { return Password; }
	FString GetLogin() const { return Login; }

private:

	FString Login;
	FString Password;
};
