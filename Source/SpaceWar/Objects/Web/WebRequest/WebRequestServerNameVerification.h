// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/WebRequestBase.h"
#include "SpaceWar/Structs/GetServerListCallBack.h"
#include "WebRequestServerNameVerification.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API UWebRequestServerNameVerification : public UWebRequestBase
{
	GENERATED_BODY()

public:

	virtual void CollectRequest(const FString& ScriptURL) override;
	void AddVerificationKey(const FString& ServerName, const FServerNameVerificationCallback& VerificationCallback);

protected:

	virtual void CallJsonResponse(const TSharedPtr<FJsonObject>& JsonResponse) override;
	virtual void CallJsonFail() override;

private:

	FString VerificationName;
	FServerNameVerificationCallback Callback;
};
