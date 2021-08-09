// Fill out your copyright notice in the Description page of Project Settings.


#include "WebRequestCreateGameServer.h"

void UWebRequestCreateGameServer::AddCreateServerKeys(const FString& MapName, const FString& MapAddress, const FCreateServerDelegate& CallBack)
{
	Address = MapName;
	LevelName = MapAddress;
	OnCreateServerDelegate = CallBack;
}

void UWebRequestCreateGameServer::CollectRequest(const FString& ScriptURL)
{
	
}

void UWebRequestCreateGameServer::CallJsonResponse(const TSharedPtr<FJsonObject>& JsonResponse)
{
	if(JsonResponse->HasField("Error"))
	{
		OnCreateServerDelegate.Execute(false, JsonResponse->GetStringField("Error"));
		return;
	}
}

void UWebRequestCreateGameServer::CallJsonFail()
{
	
}