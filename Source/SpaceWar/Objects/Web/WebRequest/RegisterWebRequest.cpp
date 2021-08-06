// Fill out your copyright notice in the Description page of Project Settings.


#include "RegisterWebRequest.h"

DEFINE_LOG_CATEGORY(RegisterWebRequestLog);

void URegisterWebRequest::AddRegisterKey(const FRegisterInfo& Info, const FDelegateRequestRegisterUserCallBack& CallBack)
{
	RegisterInfoKey = Info;
	RegisterCallBack.OnRequestRegisterUserCallBack = CallBack; 
}

void URegisterWebRequest::CollectRequest(const FString& ScriptURL)
{
	TSharedPtr<FJsonObject> JsonObject = CreateJsonRequest();
	JsonObject->SetStringField("Login", RegisterInfoKey.Login);
	JsonObject->SetStringField("Password", RegisterInfoKey.Password);
	JsonObject->SetStringField("Email", RegisterInfoKey.PlayerEmail);
	CallWebScript(ScriptURL, JsonObject);
}

void URegisterWebRequest::CallJsonFail()
{
	RegisterCallBack.OnRequestRegisterUserCallBack.ExecuteIfBound(false, "Error");
}

void URegisterWebRequest::CallJsonResponse(const TSharedPtr<FJsonObject>& JsonResponse)
{
	bool bResult = false;
	if(!JsonResponse->HasField("RegisterResult"))
	{
		UE_LOG(RegisterWebRequestLog, Error, TEXT("URegisterWebRequest::CallJsonResponse --Json return an invalid field"));
		return;
	}
	bResult = JsonResponse->GetBoolField("RegisterResult");
	FString const ErrorMessage = JsonResponse->HasField("Error") ? JsonResponse->GetStringField("Error") : "";
	RegisterCallBack.OnRequestRegisterUserCallBack.ExecuteIfBound(bResult, ErrorMessage);
}

