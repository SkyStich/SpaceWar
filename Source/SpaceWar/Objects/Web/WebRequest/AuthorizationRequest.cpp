// Fill out your copyright notice in the Description page of Project Settings.


#include "AuthorizationRequest.h"

DEFINE_LOG_CATEGORY(LogWebAuthorizationRequest);

void UAuthorizationRequest::CallJsonResponse(const TSharedPtr<FJsonObject>& JsonResponse)
{
	bool Result = false;

	if(!JsonResponse->HasField("AuthorizationResult"))
	{
		UE_LOG(LogWebAuthorizationRequest, Error, TEXT("UAuthorizationRequest::CallJsonResponse --Json return an invalid field"));
		return;
	}
	Result = JsonResponse->GetBoolField("AuthorizationResult");
	OnAuthorizationDelegate.Execute(Result);
}

void UAuthorizationRequest::CallJsonFail()
{
	
}

void UAuthorizationRequest::CollectRequest(const FString& ScriptURL)
{
	if(AuthorizationKey.IsEmpty())
	{
		UE_LOG(LogWebAuthorizationRequest, Error, TEXT("UAuthorizationRequest::CollectRequest --AuthorizationKey is empty."));
		return;
	}
	TSharedPtr<FJsonObject> JsonObject = CreateJsonRequest();
	JsonObject->SetStringField(AuthorizationKey.GetFieldName(), AuthorizationKey.GetValue());
	CallWebScript(ScriptURL, JsonObject);
}
