// Fill out your copyright notice in the Description page of Project Settings.


#include "WebRequestReceivingWeaponList.h"

void UWebRequestReceivingWeaponList::AddReceivingWeaponListKey(const FString& Login, const FReceivingWeaponListDelegate& CallBack)
{
	PlayerLogin = Login;
	ReceivingWeaponListCallBack.OnReceivingWeaponListDelegate = CallBack;
}

void UWebRequestReceivingWeaponList::CollectRequest(const FString& ScriptURL)
{
	TSharedPtr<FJsonObject> JsonObject = CreateJsonRequest();
	JsonObject->SetStringField("Login", PlayerLogin);

	CallWebScript(ScriptURL, JsonObject, EWebRequestType::Post);
}

void UWebRequestReceivingWeaponList::CallJsonResponse(const TSharedPtr<FJsonObject>& JsonResponse)
{
	TArray<FString> WeaponList;
	JsonResponse->TryGetStringArrayField("WeaponList", WeaponList);
	ReceivingWeaponListCallBack.OnReceivingWeaponListDelegate.Execute(WeaponList);
}

void UWebRequestReceivingWeaponList::CallJsonFail()
{
	UE_LOG(LogTemp, Error, TEXT("Call json fail! %s"), *GetName());
}

