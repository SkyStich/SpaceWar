// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/WebRequestBase.h"
#include "SpaceWar/Structs/CreateServerCallBack.h"

#include "WebRequestCreateHubServer.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API UWebRequestCreateHubServer : public UWebRequestBase
{
	GENERATED_BODY()
	
public:

	virtual void CollectRequest(const FString& ScriptURL) override;
	void AddCreateHudServerKey(const FString& serverName, const FString& address, const FCreateServerDelegate& callback);

protected:

	virtual void CallJsonResponse(const TSharedPtr<FJsonObject>& JsonResponse) override;
	virtual void CallJsonFail() override;

	FString ServerName;
	FString Address;
	FCreateServerDelegate Callback;
};
