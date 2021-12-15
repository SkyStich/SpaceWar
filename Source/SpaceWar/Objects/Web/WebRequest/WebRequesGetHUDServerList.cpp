// Fill out your copyright notice in the Description page of Project Settings.


#include "WebRequesGetHUDServerList.h"

void UWebRequesGetHUDServerList::AddReceivingWeaponListKey(const FGetServerHudListDelegate& CallBack)
{
	OnHudServerListCallback = CallBack;
}

void UWebRequesGetHUDServerList::CollectRequest(const FString& ScriptURL)
{
	TSharedPtr<FJsonObject> JsonObject = CreateJsonRequest();
	CallWebScript(ScriptURL, JsonObject);	
}

void UWebRequesGetHUDServerList::CallJsonResponse(const TSharedPtr<FJsonObject>& JsonResponse)
{
	TArray<FString>ServerNames;
	TArray<FString> Addresses;
	JsonResponse->TryGetStringArrayField("ServerNames", ServerNames);
	JsonResponse->TryGetStringArrayField("ServerAddresses", Addresses);

	TArray<FGetServerHUDListCallBack> ServerList;
	for(int32 i = 0; i < Addresses.Num(); i++)
	{
		ServerList.Add(FGetServerHUDListCallBack(ServerNames[i], Addresses[i]));
	}
	OnHudServerListCallback.Execute(ServerList);
}

void UWebRequesGetHUDServerList::CallJsonFail()
{
	UE_LOG(LogTemp, Error, TEXT("Call json fail! %s"), *GetName());
}
