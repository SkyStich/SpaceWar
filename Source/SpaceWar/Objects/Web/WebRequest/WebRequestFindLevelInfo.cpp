// Fill out your copyright notice in the Description page of Project Settings.


#include "WebRequestFindLevelInfo.h"

void UWebRequestFindLevelInfo::AddFindLevelInfoKeys(const FString& Login, const FFindPlayerLevel& Callback)
{
	PlayerLogin = Login;
	OnGetPlayerLevel = Callback;
}

void UWebRequestFindLevelInfo::CollectRequest(const FString& ScriptURL)
{
	TSharedPtr<FJsonObject> JsonObject = CreateJsonRequest();

	JsonObject->SetStringField("Login", PlayerLogin);
	CallWebScript(ScriptURL, JsonObject);
}

void UWebRequestFindLevelInfo::CallJsonResponse(const TSharedPtr<FJsonObject>& JsonResponse)
{
	if(JsonResponse->HasField("Error"))
	{
		UE_LOG(LogWebRequest, Error, TEXT("Find level info complete with Error!"), *JsonResponse->GetStringField("Error"));
		return;
	}
	OnGetPlayerLevel.Execute(JsonResponse->GetIntegerField("Level"), JsonResponse->GetIntegerField("Exp"));
}

void UWebRequestFindLevelInfo::CallJsonFail()
{
	UE_LOG(LogWebRequest, Error, TEXT("json request complete with fail! --%s"), *GetFullName());
}