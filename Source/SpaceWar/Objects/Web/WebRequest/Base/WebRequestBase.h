// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "WebRequestBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogWebRequest, Log, All);

UENUM(BlueprintType)
enum class EWebRequestType : uint8
{
	Post,
	Get
};

UCLASS()
class SPACEWAR_API UWebRequestBase : public UObject
{
	GENERATED_BODY()

private:

	void InitRequest(TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request, EWebRequestType WebRequestType, const FString& ScriptURL);

protected:

	/** принимает успешные ответы от веб сервера */
	virtual void CallJsonResponse(const TSharedPtr<FJsonObject>& JsonResponse) {}

	/** принимает не успешные ответы от сервера */
	virtual void CallJsonFail() {}

	virtual void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessFull);

	/** Отправка данных на веб сервер */
	bool CallWebScript(const FString& ScriptURL, TSharedPtr<FJsonObject>& JsonRequest, EWebRequestType WebRequestType = EWebRequestType::Post);

	/** создает JsonObject для веб запроса */
	TSharedPtr<FJsonObject> CreateJsonRequest();

private:

	/** модуль для создания запросов */
	FHttpModule* HttpModule;

	FString JSonStream = "JsonStream";

	bool bSSLUsed = false;
};
