// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/WebRequestBase.h"
#include "SpaceWar/Structs/CreateServerCallBack.h"

#include "WebRequestCheckServerCreation.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API UWebRequestCheckServerCreation : public UWebRequestBase
{
	GENERATED_BODY()

public:

	virtual void CollectRequest(const FString& ScriptURL) override;
	void AddCheckingServerKey(const FString& ServerName, const FCreateServerCompelete& CallBack);

protected:

	virtual void CallJsonResponse(const TSharedPtr<FJsonObject>& JsonResponse) override;
	virtual void CallJsonFail() override;

private:

	FString Name;
	FCreateServerCompelete OnCreateServerComplete;
};
