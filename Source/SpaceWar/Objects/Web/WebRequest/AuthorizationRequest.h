// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/WebRequestBase.h"
#include "SpaceWar/Structs/RegisterUsersCallBack.h"
#include "SpaceWar/Structs/UserInfo.h"

#include "AuthorizationRequest.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogWebAuthorizationRequest, Log, All);

UCLASS()
class SPACEWAR_API UAuthorizationRequest : public UWebRequestBase
{
	GENERATED_BODY()

public:

	void AddAuthorizationValue(const FUserInfo& UserInfo, FDelegateRequestRegisterUserCallBack CallBack);
	virtual void CollectRequest(const FString& ScriptURL) override;

protected:

	virtual void CallJsonResponse(const TSharedPtr<FJsonObject>& JsonResponse) override;
	virtual void CallJsonFail() override;

private:

	FUserInfo AuthorizationKey;
	FDelegateRequestRegisterUserCallBack AuthorizationUserCallBack;
};
