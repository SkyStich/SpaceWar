// Fill out your copyright notice in the Description page of Project Settings.


#include "WebRequestReceivingWeaponList.h"

void UWebRequestReceivingWeaponList::CollectRequest(const FString& ScriptURL)
{
	TSharedPtr<FJsonObject> JsonObject = CreateJsonRequest();

	CallWebScript(ScriptURL, JsonObject, EWebRequestType::Get);
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

