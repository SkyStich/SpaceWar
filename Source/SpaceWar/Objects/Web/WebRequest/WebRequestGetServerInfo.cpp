// Fill out your copyright notice in the Description page of Project Settings.


#include "WebRequestGetServerInfo.h"

DEFINE_LOG_CATEGORY(LogWebRequestServerInfo);

void UWebRequestGetServerInfo::AddServerInfoKey(int32 Id, const FGetServerInfoDelegate& CallBack)
{
	ServerID = Id;
	ServerInfoCallBack.OnGetServerInfoDelegate = CallBack;
}

void UWebRequestGetServerInfo::CollectRequest(const FString& ScriptURL)
{
	TSharedPtr<FJsonObject> JsonObject = CreateJsonRequest();
	JsonObject->SetStringField("id", FString::FromInt(ServerID));
	CallWebScript(ScriptURL, JsonObject);
}

void UWebRequestGetServerInfo::CallJsonResponse(const TSharedPtr<FJsonObject>& JsonResponse)
{
	if(JsonResponse->HasField("Error"))
	{
		ServerInfoCallBack.OnGetServerInfoDelegate.ExecuteIfBound(false, JsonResponse->GetStringField("Error"), FServersData());
		return;
	}
	FServersData Data;
	Data.Name = JsonResponse->GetStringField("ServerName");
	Data.Address = JsonResponse->GetStringField("Address");
	UE_LOG(LogWebRequestServerInfo, Warning, TEXT("Server Info Name: %s     Address: %s"), *Data.Name, *Data.Address);
	ServerInfoCallBack.OnGetServerInfoDelegate.Execute(true, "", Data);
}

void UWebRequestGetServerInfo::CallJsonFail()
{
	UE_LOG(LogWebRequestServerInfo, Error, TEXT("Get server info is fail!"), *GetName());
}
