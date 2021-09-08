// Fill out your copyright notice in the Description page of Project Settings.


#include "WebRequestCheckServerCreation.h"

void UWebRequestCheckServerCreation::AddCheckingServerKey(const FString& ServerName, const FCreateServerCompelete& CallBack)
{
	Name = ServerName;
	OnCreateServerComplete = CallBack;
}

void UWebRequestCheckServerCreation::CollectRequest(const FString& ScriptURL)
{
	TSharedPtr<FJsonObject> JsonObject = CreateJsonRequest();

	JsonObject->SetStringField("ServerName", Name);
	CallWebScript(ScriptURL, JsonObject);
}

void UWebRequestCheckServerCreation::CallJsonResponse(const TSharedPtr<FJsonObject>& JsonResponse)
{
	FString const Address = JsonResponse->HasField("ip") ? JsonResponse->GetStringField("ip") : "";
	bool const bResult = JsonResponse->GetBoolField("Result");
	OnCreateServerComplete.Execute(bResult, Address);
}

void UWebRequestCheckServerCreation::CallJsonFail()
{
	
}
