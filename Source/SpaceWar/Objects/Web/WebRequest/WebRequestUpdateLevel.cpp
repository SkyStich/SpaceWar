// Fill out your copyright notice in the Description page of Project Settings.


#include "WebRequestUpdateLevel.h"

void UWebRequestUpdateLevel::AddUpdateLevelKey(const int32 NewExp, const FString& Name)
{
	Exp = NewExp;
	PlayerName = Name;
}

void UWebRequestUpdateLevel::CollectRequest(const FString& ScriptURL)
{
	TSharedPtr<FJsonObject> JsonObject = CreateJsonRequest();

	JsonObject->SetNumberField("Exp", Exp);
	JsonObject->SetStringField("PlayerName", PlayerName);
	CallWebScript(ScriptURL, JsonObject);
}

void UWebRequestUpdateLevel::CallJsonResponse(const TSharedPtr<FJsonObject>& JsonResponse)
{
	if(JsonResponse->HasField("Error"))
	{
		FString const ErrorMessage = JsonResponse->GetStringField("Error");
		UE_LOG(LogWebRequest, Error, TEXT("Update level info complete with fail: %s"), *ErrorMessage);
		return;
	}
	UE_LOG(LogWebRequest, Warning, TEXT("Update level complete"));
}

void UWebRequestUpdateLevel::CallJsonFail()
{
	UE_LOG(LogTemp, Error, TEXT("Call json fail! %s"), *GetName());
}
