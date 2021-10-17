// Fill out your copyright notice in the Description page of Project Settings.


#include "WebRequestCreateGameServer.h"

void UWebRequestCreateGameServer::AddCreateServerKeys(const FString& Name, const FString& MapAddress, const FString& NewMapName, const FString& ServerOwnerName, const FCreateServerDelegate& CallBack)
{
	Address = MapAddress;
	ServerName = Name;
	MapName = NewMapName;
	OwnerName = ServerOwnerName;
	OnCreateServerDelegate = CallBack;
}

void UWebRequestCreateGameServer::CollectRequest(const FString& ScriptURL)
{
	TSharedPtr<FJsonObject> JsonObject = CreateJsonRequest();
	JsonObject->SetStringField("Servername", ServerName);
	JsonObject->SetStringField("MapName", MapName);
	JsonObject->SetStringField("Address", Address);
	JsonObject->SetStringField("HudName", OwnerName);

	CallWebScript(ScriptURL, JsonObject);
}

void UWebRequestCreateGameServer::CallJsonResponse(const TSharedPtr<FJsonObject>& JsonResponse)
{
	if(JsonResponse->HasField("Error"))
	{
		OnCreateServerDelegate.Execute(-1, JsonResponse->GetStringField("Error"));
		return;
	}
	int32 const Result = JsonResponse->GetIntegerField("ServerId");
	OnCreateServerDelegate.Execute(Result, "");
}

void UWebRequestCreateGameServer::CallJsonFail()
{
	UE_LOG(LogTemp, Error, TEXT("Call json field %s: "), *GetName());
}