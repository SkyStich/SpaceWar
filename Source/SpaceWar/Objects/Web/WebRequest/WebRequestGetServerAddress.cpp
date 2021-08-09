// Fill out your copyright notice in the Description page of Project Settings.


#include "WebRequestGetServerAddress.h"

DEFINE_LOG_CATEGORY(LogWebRequestServerAddress);

void UWebRequestGetServerAddress::CollectRequest(const FString& ScriptURL)
{
	UE_LOG(LogWebRequestServerAddress, Warning, TEXT("CollectRequest"));
	TSharedPtr<FJsonObject> JsonObject = CreateJsonRequest();
	CallWebScript(ScriptURL, JsonObject);
}

void UWebRequestGetServerAddress::CallJsonResponse(const TSharedPtr<FJsonObject>& JsonResponse)
{
	if(JsonResponse->HasField("Error"))
	{
		GameAddressCallBack.OnGameServerAddress.Execute("", JsonResponse->GetStringField("Error"));

		UE_LOG(LogWebRequestServerAddress, Error, TEXT("%s: --Call json response have error field"), *GetName());
		return;
	}
	UE_LOG(LogWebRequestServerAddress, Warning, TEXT("%s: --Call json response successess: %s"), *GetName(), *JsonResponse->GetStringField("ip"));
	
	GameAddressCallBack.OnGameServerAddress.Execute(JsonResponse->GetStringField("ip"), "");
}

void UWebRequestGetServerAddress::CallJsonFail()
{
	GameAddressCallBack.OnGameServerAddress.Execute("", "Call JsonHas Field");
	UE_LOG(LogWebRequestServerAddress, Error, TEXT("%s: --Call json response has field"), *GetName());
}

void UWebRequestGetServerAddress::AddServerAddressKey(const FGameServerAddress& CallBack)
{
	GameAddressCallBack.OnGameServerAddress = CallBack;
}

