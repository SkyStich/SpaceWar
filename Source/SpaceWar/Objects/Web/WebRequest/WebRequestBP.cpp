// Fill out your copyright notice in the Description page of Project Settings.


#include "WebRequestBP.h"

void UWebRequestBP::CallJsonResponse(const TSharedPtr<FJsonObject>& JsonResponse)
{
	FString ResultString = "";
	double ResultFloat = 0.f;

	/** обработка входящего ответа */
	if(!JsonResponse->HasField("ResultText")) return;

	ResultFloat = JsonResponse->GetNumberField("ResultFloat");
	ResultString = JsonResponse->GetStringField("ResultString");

	UE_LOG(LogTemp, Warning, TEXT("Web response: string = '%s', number = '%f'"), *ResultString, ResultFloat);

	OnResultTest.Broadcast(ResultString, ResultFloat);
}

void UWebRequestBP::CallJsonFail()
{
	
}

void UWebRequestBP::BP_CallWebScript(const FString& URL)
{
	TSharedPtr<FJsonObject> JsonObject = CreateJsonRequest();

	/** Write Key num in json */
	if(KeyNumbers.Num() > 0)
	{
		for(const auto& ByArray : KeyNumbers)
		{
			JsonObject->SetNumberField(ByArray.Key, ByArray.Value);
		}
	}

	/** Write key numstring in json */
	if(KeyStrings.Num() > 0)
	{
		for(const auto& ByArray : KeyStrings)
		{
			JsonObject->SetStringField(ByArray.Key, ByArray.Value);
		}
	}

	/** drop json on server */
	CallWebScript(URL, JsonObject);
}
