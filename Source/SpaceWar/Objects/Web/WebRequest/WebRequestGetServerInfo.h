// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/WebRequestBase.h"
#include "SpaceWar/Structs/ServerInfo.h"

#include "WebRequestGetServerInfo.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogWebRequestServerInfo, Log, All);

UCLASS()
class SPACEWAR_API UWebRequestGetServerInfo : public UWebRequestBase
{
	GENERATED_BODY()

public:

	virtual void CollectRequest(const FString& ScriptURL) override;
	void AddServerInfoKey(int32 Id, const FGetServerInfoDelegate& CallBack);
	
protected:

	virtual void CallJsonResponse(const TSharedPtr<FJsonObject>& JsonResponse) override;
	virtual void CallJsonFail() override;

private:

	FGetServerInfoCallBack ServerInfoCallBack;
	int32 ServerID;
};
