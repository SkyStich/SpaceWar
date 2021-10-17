// Fill out your copyright notice in the Description page of Project Settings.


#include "WebRequestCreateHubServer.h"

void UWebRequestCreateHubServer::AddCreateHudServerKey(const FString& serverName, const FString& address, const FCreateServerDelegate& callback)
{
	ServerName = serverName;
	Address = address;
	Callback = callback;
}

void UWebRequestCreateHubServer::CollectRequest(const FString& ScriptURL)
{
	TSharedPtr<FJsonObject> JsonObject = CreateJsonRequest();

	JsonObject->SetStringField("Servername", ServerName);
	JsonObject->SetStringField("Address", Address);

	CallWebScript(ScriptURL, JsonObject);
}

void UWebRequestCreateHubServer::CallJsonResponse(const TSharedPtr<FJsonObject>& JsonResponse)
{
	if(JsonResponse->HasField("Error"))
	{
		Callback.Execute(-1, JsonResponse->GetStringField("Error"));
		return;
	}
	int32 const Result = JsonResponse->GetIntegerField("ServerId");
	Callback.Execute(Result, "");
}

void UWebRequestCreateHubServer::CallJsonFail()
{
	UE_LOG(LogTemp, Error, TEXT("Call json field %s: "), *GetName());
}



