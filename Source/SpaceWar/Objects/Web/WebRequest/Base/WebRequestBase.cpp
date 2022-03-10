// Fill out your copyright notice in the Description page of Project Settings.


#include "WebRequestBase.h"

DEFINE_LOG_CATEGORY(LogWebRequest);

TSharedPtr<FJsonObject> UWebRequestBase::CreateJsonRequest()
{
	return MakeShareable(new FJsonObject);
}

bool UWebRequestBase::CallWebScript(const FString& ScriptURL, TSharedPtr<FJsonObject>& JsonRequest, EWebRequestType WebRequestType)
{
	if(!HttpModule) HttpModule = &FHttpModule::Get();

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe>HttpRequest = HttpModule->CreateRequest();

	FString ResultURL = bSSLUsed ? "https://" : "http://";

	ResultURL += ScriptURL;

	InitRequest(HttpRequest, WebRequestType, ResultURL);

	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&JSonStream);

	FJsonSerializer::Serialize(JsonRequest.ToSharedRef(), JsonWriter);
	HttpRequest->SetContentAsString(JSonStream);

	UE_LOG(LogWebRequest, Warning, TEXT("Request Json Data to '%s'."), *ResultURL);

	HttpRequest->ProcessRequest();

	return true;
}

void UWebRequestBase::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessFull)
{
	if(!WasSuccessFull)
	{
		CallJsonFail();
		return;
	}
	
	TSharedPtr<FJsonObject>JsonObject;
	TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(Response->GetContentAsString());

	UE_LOG(LogWebRequest, Warning, TEXT("--OnResponseReceived Respose json: \t%s"), *Response->GetContentAsString());

	if(FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		UE_LOG(LogWebRequest, Warning, TEXT("------------------------------------------------------"));
		UE_LOG(LogWebRequest, Warning, TEXT("Deserialize complete  --OnResponseReceived"));
		UE_LOG(LogWebRequest, Warning, TEXT("------------------------------------------------------"));
		
		CallJsonResponse(JsonObject);
	}
	else
	{
		UE_LOG(LogWebRequest, Error, TEXT("Deserialize complete with fail --OnResponseReceived"));
		CallJsonFail();
	}
}

void UWebRequestBase::InitRequest(TSharedRef<IHttpRequest, ESPMode::ThreadSafe>& Request, EWebRequestType WebRequestType, const FString& ScriptURL)
{
	/** Bind on request complete */
	Request->OnProcessRequestComplete().BindUObject(this, &UWebRequestBase::OnResponseReceived);

	/** set address url */
	Request->SetURL(ScriptURL);
	FString Verb = WebRequestType == EWebRequestType::Post ? "POST" : "GET";
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/Json"));
}

