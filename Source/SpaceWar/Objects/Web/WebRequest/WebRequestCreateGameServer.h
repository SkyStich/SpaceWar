// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/WebRequestBase.h"
#include "SpaceWar/Structs/CreateServerCallBack.h"
#include "WebRequestCreateGameServer.generated.h"

USTRUCT()
struct FCreateServerData
{
	GENERATED_BODY()

	FCreateServerData() {}
	FCreateServerData(const FString& NewDisplayName, const FString& NewMapName, const FString& NewAddress)
	: DisplayName(NewDisplayName), MapName(NewMapName), Address(NewAddress) {}
	
	FString DisplayName;
	FString MapName;
	FString Address;
};

UCLASS()
class SPACEWAR_API UWebRequestCreateGameServer : public UWebRequestBase
{
	GENERATED_BODY()

public:

	virtual void CollectRequest(const FString& ScriptURL) override;
	void AddCreateServerKeys(FCreateServerData CreateServerData, const FCreateServerDelegate& CallBack);
	
protected:

	virtual void CallJsonResponse(const TSharedPtr<FJsonObject>& JsonResponse) override;
	virtual void CallJsonFail() override;

private:

	FCreateServerDelegate OnCreateServerDelegate;
	FCreateServerData ServerData;
};
