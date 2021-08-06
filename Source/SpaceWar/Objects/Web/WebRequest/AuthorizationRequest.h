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

	bool IsEmpty() const { return Login.IsEmpty() || Pass.IsEmpty(); }
	FKeyAuthorization() : Login(""), Pass("") {}
	FKeyAuthorization(const FString& NewKey, const FString& NewValue) : Login(NewKey), Pass(NewValue) {}

	FString GetLogin() const { return Login; }
	FString GetPass() const { return Pass; }

private:

	FString Login; //Login
	FString Pass; //Pass
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
