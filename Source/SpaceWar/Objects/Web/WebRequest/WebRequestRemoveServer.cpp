// Fill out your copyright notice in the Description page of Project Settings.


#include "WebRequestRemoveServer.h"

DEFINE_LOG_CATEGORY(LogWebRequestRemoveServer);

void UWebRequestRemoveServer::AddRemoveServerKey(int32 RemovedId, const FRemoveServerDelegate& CallBack)
{
	Id = RemovedId;
	RemoveServerCallBack.OnRemoveServerDelegate = CallBack;
}

void UWebRequestRemoveServer::CollectRequest(const FString& ScriptURL)
{
	TSharedPtr<FJsonObject> JsonObject = CreateJsonRequest();

	JsonObject->SetNumberField("id", Id);

	CallWebScript(ScriptURL, JsonObject);
}

void UWebRequestRemoveServer::CallJsonResponse(const TSharedPtr<FJsonObject>& JsonResponse)
{
	bool const bResult = JsonResponse->GetBoolField("Result");
	RemoveServerCallBack.OnRemoveServerDelegate.Execute(bResult, JsonResponse->HasField("Error") ? JsonResponse->GetStringField("Error") : "");
}

void UWebRequestRemoveServer::CallJsonFail()
{
	UE_LOG(LogWebRequestRemoveServer, Error, TEXT("Call json fail! %s"), *GetName());
}