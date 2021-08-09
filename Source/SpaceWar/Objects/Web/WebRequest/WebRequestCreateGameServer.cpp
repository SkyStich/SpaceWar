// Fill out your copyright notice in the Description page of Project Settings.


#include "WebRequestCreateGameServer.h"

void UWebRequestCreateGameServer::AddCreateServerKeys(FCreateServerData CreateServerData, const FCreateServerDelegate& CallBack)
{
	ServerData.Address = CreateServerData.Address;
	ServerData.DisplayName = CreateServerData.DisplayName;
	ServerData.MapName = CreateServerData.MapName;
	OnCreateServerDelegate = CallBack;
}

void UWebRequestCreateGameServer::CollectRequest(const FString& ScriptURL)
{
	
}

void UWebRequestCreateGameServer::CallJsonResponse(const TSharedPtr<FJsonObject>& JsonResponse)
{
	if(JsonResponse->HasField("Error"))
	{
		OnCreateServerDelegate.ExecuteIfBound(false);
		return;
	}
}

void UWebRequestCreateGameServer::CallJsonFail()
{
	
}