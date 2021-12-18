// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/WebRequestBase.h"
#include "SpaceWar/PlayerControllers/Match/Base/MatchPlayerControllerBase.h"

#include "WebRequestFindLevelInfo.generated.h"

UCLASS()
class SPACEWAR_API UWebRequestFindLevelInfo : public UWebRequestBase
{
	GENERATED_BODY()

public:

	virtual void CollectRequest(const FString& ScriptURL) override;
	void AddFindLevelInfoKeys(const FString& Login, const FFindPlayerLevel& Callback);

protected:

	virtual void CallJsonResponse(const TSharedPtr<FJsonObject>& JsonResponse) override;
	virtual void CallJsonFail() override;

private:

	FString PlayerLogin;
	FFindPlayerLevel OnGetPlayerLevel;
};
