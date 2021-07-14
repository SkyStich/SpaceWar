// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/WebRequestBase.h"
#include "WebRequestBP.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FResultTest, const FString&, StringValue, float, FloatValue);

/** struct json of requests with float(int) value */
USTRUCT()
struct FKeyNumber
{
	GENERATED_BODY()
	
	FString Key;
	float Value;

	FKeyNumber() : Key(""), Value(0.f) {}
	FKeyNumber(const FString& NewKey, float NewValue) : Key(NewKey), Value(NewValue) {} 
};

/** struct json requests with string value */
USTRUCT()
struct FKeyString
{
	GENERATED_BODY()

	FString Key;
	FString Value;

	FKeyString() : Key(""), Value("") {}
	FKeyString(const FString& NewKey, const FString& NewValue) : Key(NewKey), Value(NewValue) {}
};

UCLASS(Blueprintable)
class SPACEWAR_API UWebRequestBP : public UWebRequestBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "WebRequest", meta = (DisplayName = "Add Number (BP)"))
	void BP_AddNumber(const FString& Key, float Value) { KeyNumbers.Add(FKeyNumber(Key, Value)); }

	UFUNCTION(BlueprintCallable, Category = "WebRequest", meta = (DisplayName = "Add String (BP)"))
	void BP_AddString(const FString& Key, const FString& Value) { KeyStrings.Add(FKeyString(Key, Value)); }

	UFUNCTION(BlueprintCallable, Category = "WebRequest", DisplayName = "Add web script (BP)")
	void BP_CallWebScript(const FString& URL);

protected:

	virtual void CallJsonResponse(const TSharedPtr<FJsonObject>& JsonResponse) override;
	virtual void CallJsonFail() override;

public:

	UPROPERTY(BlueprintAssignable, Category = "WebRequest|Delegates")
	FResultTest OnResultTest;

private:

	UPROPERTY()
	TArray<FKeyString> KeyStrings;

	UPROPERTY()
	TArray<FKeyNumber> KeyNumbers;
};
