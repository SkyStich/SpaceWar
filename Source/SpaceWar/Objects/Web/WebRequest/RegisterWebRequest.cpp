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

	/** Create session hash */
	int64 const NowTime = FDateTime::Now().ToUnixTimestamp();
	FString const TimeToString = FString::Printf(TEXT("%lld"), NowTime);
	FString const LeftHash = FMD5::HashAnsiString(*(RegisterInfoKey.Login + TimeToString));
	FString const RightHash = FMD5::HashAnsiString(*TimeToString);
	NewSessionID = LeftHash + RightHash;

	/** Set JsonObjectFields */
	JsonObject->SetStringField("Login", RegisterInfoKey.Login);
	JsonObject->SetStringField("Password", RegisterInfoKey.Password);
	JsonObject->SetStringField("Email", RegisterInfoKey.PlayerEmail);
	JsonObject->SetStringField("SessionID", NewSessionID);
	JsonObject->SetNumberField("SessionTime", (double)NowTime);
	
	CallWebScript(ScriptURL, JsonObject);
}

void URegisterWebRequest::CallJsonFail()
{
	RegisterCallBack.OnRequestRegisterUserCallBack.Execute(false, "", "URegisterWebRequest::CallJsonFail");
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
	RegisterCallBack.OnRequestRegisterUserCallBack.Execute(bResult, NewSessionID, ErrorMessage);
}

