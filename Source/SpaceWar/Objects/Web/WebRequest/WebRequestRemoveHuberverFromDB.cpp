// Fill out your copyright notice in the Description page of Project Settings.


#include "WebRequestRemoveHubServerFromDB.h"

void UWebRequestRemoveHubServerFromDB::AddRemoveServerKey(int32 RemovedId, const FRemoveServerDelegate& CallBack)
{
	Id = RemovedId;
	RemoveServerCallBack.OnRemoveServerDelegate = CallBack;
}

void UWebRequestRemoveHubServerFromDB::CollectRequest(const FString& ScriptURL)
{
	TSharedPtr<FJsonObject> JsonObject = CreateJsonRequest();

	JsonObject->SetNumberField("id", Id);

	CallWebScript(ScriptURL, JsonObject);
}

void UWebRequestRemoveHubServerFromDB::CallJsonResponse(const TSharedPtr<FJsonObject>& JsonResponse)
{
	bool const bResult = JsonResponse->GetBoolField("Result");
	RemoveServerCallBack.OnRemoveServerDelegate.Execute(bResult, JsonResponse->HasField("Error") ? JsonResponse->GetStringField("Error") : "");
}

void UWebRequestRemoveHubServerFromDB::CallJsonFail()
{
	UE_LOG(LogWebRequest, Error, TEXT("Call json fail! %s"), *GetName());
}


