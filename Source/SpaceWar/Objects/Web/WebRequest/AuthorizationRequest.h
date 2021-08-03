// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/WebRequestBase.h"
#include "AuthorizationRequest.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogWebAuthorizationRequest, Log, All);

DECLARE_DELEGATE_OneParam(FAuthorizationDelegate, bool /** result authorization */);

/** struct json of request on authorization */
USTRUCT()
struct FKeyAuthorization
{
	GENERATED_BODY()

	bool IsEmpty() const { return FieldName.IsEmpty() || Value.IsEmpty(); }
	FKeyAuthorization() : FieldName(""), Value("") {}
	FKeyAuthorization(const FString& NewKey, const FString& NewValue) : FieldName(NewKey), Value(NewValue) {}

	FString GetFieldName() const { return FieldName; }
	FString GetValue() const { return Value; }

private:

	FString FieldName; //Login
	FString Value; //Pass
};

UCLASS()
class SPACEWAR_API UAuthorizationRequest : public UWebRequestBase
{
	GENERATED_BODY()

public:

	void AddAuthorizationValue(const FString& Key, const FString& Value) { AuthorizationKey = FKeyAuthorization(Key, Value); }
	virtual void CollectRequest(const FString& ScriptURL) override;

protected:

	virtual void CallJsonResponse(const TSharedPtr<FJsonObject>& JsonResponse) override;
	virtual void CallJsonFail() override;

private:

	FKeyAuthorization AuthorizationKey;

public:

	FAuthorizationDelegate OnAuthorizationDelegate;
};
