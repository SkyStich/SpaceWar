// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/WebRequestBase.h"
#include "SpaceWar/Structs/GetServerListCallBack.h"

#include "WebRequestGetServerList.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API UWebRequestGetServerList : public UWebRequestBase
{
	GENERATED_BODY()

public:

	virtual void CollectRequest(const FString& ScriptURL) override;
	void AddServerListKey(const TArray<FString>& MapNames, const FGetServerListDelegate& CallBack);

protected:

	virtual void CallJsonResponse(const TSharedPtr<FJsonObject>& JsonResponse) override;
	virtual void CallJsonFail() override;

private:

	TArray<FString> ServerNames;
	FGetServerListDelegate OnServerListDelegate;
};
