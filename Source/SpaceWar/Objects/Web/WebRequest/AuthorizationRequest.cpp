// Fill out your copyright notice in the Description page of Project Settings.


#include "AuthorizationRequest.h"

DEFINE_LOG_CATEGORY(LogWebAuthorizationRequest);

void UAuthorizationRequest::AddAuthorizationValue(const FUserInfo& UserInfo, FDelegateRequestRegisterUserCallBack CallBack)
{
	AuthorizationKey = UserInfo;
	AuthorizationUserCallBack = CallBack;
}

void UAuthorizationRequest::CallJsonResponse(const TSharedPtr<FJsonObject>& JsonResponse)
{
	bool Result = false;

	if(!JsonResponse->HasField("AuthorizationResult"))
	{
		UE_LOG(LogWebAuthorizationRequest, Error, TEXT("UAuthorizationRequest::CallJsonResponse --Json return an invalid field"));
		return;
	}
	Result = JsonResponse->GetBoolField("AuthorizationResult");
	AuthorizationUserCallBack.ExecuteIfBound(Result, JsonResponse->HasField("Error") ? JsonResponse->GetStringField("Error") : "");
}

void UAuthorizationRequest::CallJsonFail()
{
	AuthorizationUserCallBack.ExecuteIfBound(false, "Authorization failed");
}

void UAuthorizationRequest::CollectRequest(const FString& ScriptURL)
{
	TSharedPtr<FJsonObject> JsonObject = CreateJsonRequest();
	JsonObject->SetStringField("Login", AuthorizationKey.Login);
	JsonObject->SetStringField("Password", AuthorizationKey.Password);
	CallWebScript(ScriptURL, JsonObject);
}
