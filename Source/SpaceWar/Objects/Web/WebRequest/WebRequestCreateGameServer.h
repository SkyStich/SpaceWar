// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/WebRequestBase.h"
#include "SpaceWar/Structs/CreateServerCallBack.h"
#include "WebRequestCreateGameServer.generated.h"

UCLASS()
class SPACEWAR_API UWebRequestCreateGameServer : public UWebRequestBase
{
	GENERATED_BODY()

public:

	virtual void CollectRequest(const FString& ScriptURL) override;
	void AddCreateServerKeys(const FString& Name, const FString& MapAddress, const FString& NewMapName, const FCreateServerDelegate& CallBack);
	
protected:

	virtual void CallJsonResponse(const TSharedPtr<FJsonObject>& JsonResponse) override;
	virtual void CallJsonFail() override;

private:

	FCreateServerDelegate OnCreateServerDelegate;
	
	FString ServerName;
	FString Address;
	FString MapName;
};
