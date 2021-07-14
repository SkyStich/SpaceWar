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

	/** Засовываем все в json */
	FJsonSerializer::Serialize(JsonRequest.ToSharedRef(), JsonWriter);
	HttpRequest->SetContentAsString(JSonStream);

	UE_LOG(LogWebRequest, Warning, TEXT("Request Json Data to '%s'."), *ResultURL);

	HttpRequest->ProcessRequest();

	return true;
}

void UWebRequestBase::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessFull)
{
	if(WasSuccessFull)
	{
		TSharedPtr<FJsonObject>JsonObject;
		TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(Response->GetContentAsString());

		UE_LOG(LogWebRequest, Warning, TEXT("--OnResponseReceived Respose json: \t%s"), *Response->GetContentAsString());

		if(FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			CallJsonResponse(JsonObject);
		}
		else
		{
			UE_LOG(LogWebRequest, Error, TEXT("Deserialize complete with fail --OnResponseReceived"));
			CallJsonFail();
		}
		return;
	}
	CallJsonFail();
}

void UWebRequestBase::InitRequest(TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request, EWebRequestType WebRequestType, const FString& ScriptURL)
{
	/** Bind on request complete */
	Request->OnProcessRequestComplete().BindUObject(this, &UWebRequestBase::OnResponseReceived);

	/** set address url */
	Request->SetURL(ScriptURL);
	
	Request->SetVerb(WebRequestType == EWebRequestType::Post ? "Post" : "Get");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/Json"));
}

