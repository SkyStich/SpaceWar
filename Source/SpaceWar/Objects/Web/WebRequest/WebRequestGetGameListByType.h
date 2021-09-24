// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/WebRequestBase.h"
#include "SpaceWar/Structs/GetServerListCallBack.h"
#include "SpaceWar/Structs/ServerInfo.h"

#include "WebRequestGetGameListByType.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API UWebRequestGetGameListByType : public UWebRequestBase
{
	GENERATED_BODY()

public:

	virtual void CollectRequest(const FString& ScriptURL) override;
	void AddMapTypeKey(const FString& MapType, const FGetServerListDelegate& OnServerInfo);
	
protected:

	virtual void CallJsonResponse(const TSharedPtr<FJsonObject>& JsonResponse) override;
	virtual void CallJsonFail() override;

private:

	FString GameType;
	FGetServerListDelegate Callback;
};
