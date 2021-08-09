// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/WebRequestBase.h"
#include "SpaceWar/Structs/CreateServerCallBack.h"
#include "WebRequestGetServerAddress.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogWebRequestServerAddress, Log, All);

UCLASS()
class SPACEWAR_API UWebRequestGetServerAddress : public UWebRequestBase
{
	GENERATED_BODY()

public:

	virtual void CollectRequest(const FString& ScriptURL) override;
	void AddServerAddressKey(const FGameServerAddress& CallBack);

protected:

	virtual void CallJsonResponse(const TSharedPtr<FJsonObject>& JsonResponse) override;
	virtual void CallJsonFail() override;

private:

	FGameAddressCallBack GameAddressCallBack;
};
