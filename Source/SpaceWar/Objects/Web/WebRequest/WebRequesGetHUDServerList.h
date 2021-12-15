// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/WebRequestBase.h"
#include "SpaceWar/Structs/GetServerListCallBack.h"

#include "WebRequesGetHUDServerList.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API UWebRequesGetHUDServerList : public UWebRequestBase
{
	GENERATED_BODY()

public:
	
	virtual void CollectRequest(const FString& ScriptURL) override;
	void AddReceivingWeaponListKey(const FGetServerHudListDelegate& CallBack);

protected:

	virtual void CallJsonResponse(const TSharedPtr<FJsonObject>& JsonResponse) override;
	virtual void CallJsonFail() override;

private:

	FGetServerHudListDelegate OnHudServerListCallback;
};
