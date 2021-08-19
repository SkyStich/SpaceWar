// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/WebRequestBase.h"
#include "SpaceWar/Structs/ReceivingWeaponListCallBack.h"
#include "WebRequestReceivingWeaponList.generated.h"

UCLASS()
class SPACEWAR_API UWebRequestReceivingWeaponList : public UWebRequestBase
{
	GENERATED_BODY()
	
public:

	virtual void CollectRequest(const FString& ScriptURL) override;
	void AddReceivingWeaponListKey(const FString& Login, const FReceivingWeaponListDelegate& CallBack);

protected:

	virtual void CallJsonResponse(const TSharedPtr<FJsonObject>& JsonResponse) override;
	virtual void CallJsonFail() override;

private:

	FString PlayerLogin;
	FReceivingWeaponListCallBack ReceivingWeaponListCallBack;
};
