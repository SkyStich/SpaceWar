// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/WebRequestBase.h"
#include "WebRequestUpdateLevel.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API UWebRequestUpdateLevel : public UWebRequestBase
{
	GENERATED_BODY()


public:

	virtual void CollectRequest(const FString& ScriptURL) override;
	void AddUpdateLevelKey(const int32 NewExp, const FString& Name);

protected:

	virtual void CallJsonFail() override;
	virtual void CallJsonResponse(const TSharedPtr<FJsonObject>& JsonResponse) override;

private:

	int32 Exp;
	FString PlayerName;
};
