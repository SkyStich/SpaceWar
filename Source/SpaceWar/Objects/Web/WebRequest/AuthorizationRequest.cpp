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
	bool ExecuteResult = AuthorizationUserCallBack.ExecuteIfBound(Result, NewSessionKey, JsonResponse->HasField("Error") ? JsonResponse->GetStringField("Error") : "");
}

void UAuthorizationRequest::CallJsonFail()
{
	bool ResultExecute = AuthorizationUserCallBack.ExecuteIfBound(false, "", "Authorization failed");
}

void UAuthorizationRequest::CollectRequest(const FString& ScriptURL)
{
	TSharedPtr<FJsonObject> JsonObject = CreateJsonRequest();

	/** Create session hash */
	int64 const NowTime = FDateTime::Now().ToUnixTimestamp();
	FString const TimeToString = FString::Printf(TEXT("%lld"), NowTime);
	FString const LeftHash = FMD5::HashAnsiString(*(AuthorizationKey.Login + TimeToString));
	FString const RightHash = FMD5::HashAnsiString(*TimeToString);
	NewSessionKey = LeftHash + RightHash;
	
	JsonObject->SetStringField("Login", AuthorizationKey.Login);
	JsonObject->SetStringField("Password", AuthorizationKey.Password);
	JsonObject->SetNumberField("SessionTime", (double)NowTime);
	JsonObject->SetStringField("SessionID", NewSessionKey);
	
	CallWebScript(ScriptURL, JsonObject);
}
