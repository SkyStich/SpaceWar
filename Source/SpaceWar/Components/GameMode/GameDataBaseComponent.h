// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameServerDataBaseComponent.h"
#include "GameDataBaseComponent.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API UGameDataBaseComponent : public UGameServerDataBaseComponent
{
	GENERATED_BODY()

protected:

	virtual void OnResponseGetServerInfo(bool bResult, const FString& ErrorMessage, const FServersData& Data) override;

public:

	virtual void CreateServerInDataBase() override;
	virtual void RemoveServerFromDataBase() override;
	virtual bool UpdateServerData() override;
};
