// Fill out your copyright notice in the Description page of Project Settings.


#include "WebRequestServerNameVerification.h"

void UWebRequestServerNameVerification::AddVerificationKey(const FString& ServerName, const FServerNameVerificationCallback& VerificationCallback)
{
	VerificationName = ServerName;
	Callback = VerificationCallback;
}

void UWebRequestServerNameVerification::CollectRequest(const FString& ScriptURL)
{
	TSharedPtr<FJsonObject> JsonObject = CreateJsonRequest();

	JsonObject->SetStringField("ServerName", VerificationName);

	CallWebScript(ScriptURL, JsonObject);
}

void UWebRequestServerNameVerification::CallJsonResponse(const TSharedPtr<FJsonObject>& JsonResponse)
{
	Super::CallJsonResponse(JsonResponse);

	Callback.Execute(JsonResponse->GetBoolField("IsPassedVerification"));
}

void UWebRequestServerNameVerification::CallJsonFail()
{
	UE_LOG(LogTemp, Error, TEXT("Json fail"));
}

