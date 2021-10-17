// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/WebRequestBase.h"
#include "SpaceWar/Structs/RemoveServerCallBack.h"

#include "WebRequestRemoveHubServerFromDB.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API UWebRequestRemoveHubServerFromDB : public UWebRequestBase
{
	GENERATED_BODY()

public:

	virtual void CollectRequest(const FString& ScriptURL) override;
	void AddRemoveServerKey(int32 RemovedId, const FRemoveServerDelegate& CallBack);

protected:

	virtual void CallJsonResponse(const TSharedPtr<FJsonObject>& JsonResponse) override;
	virtual void CallJsonFail() override;
	
private:

	FRemoveServerCallBack RemoveServerCallBack;
	int32 Id;
};
