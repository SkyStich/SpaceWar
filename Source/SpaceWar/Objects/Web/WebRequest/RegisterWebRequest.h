// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/WebRequestBase.h"
#include "SpaceWar/Structs/RegisterUsersCallBack.h"
#include "SpaceWar/Structs/UserInfo.h"
#include "RegisterWebRequest.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(RegisterWebRequestLog, Log, All);

UCLASS()
class SPACEWAR_API URegisterWebRequest : public UWebRequestBase
{
	GENERATED_BODY()

public:

	void AddRegisterKey(const FRegisterInfo& Info, const FDelegateRequestRegisterUserCallBack& CallBack);
	virtual void CollectRequest(const FString& ScriptURL) override;
	
protected:

	virtual void CallJsonResponse(const TSharedPtr<FJsonObject>& JsonResponse) override;
	virtual void CallJsonFail() override;
	

private:

	FString NewSessionID;
	FRegisterInfo RegisterInfoKey;
	FCallBackRequestRegisterUser RegisterCallBack;
};
