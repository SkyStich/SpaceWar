// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameServerDataBaseComponent.h"
#include "HubServerDataBaseComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SPACEWAR_API UHubServerDataBaseComponent : public UGameServerDataBaseComponent
{
	GENERATED_BODY()

private:

	UFUNCTION()
	void DeactivateServer();

	UFUNCTION()
	void ActivateServer();

protected:

	virtual void OnResponseGetServerInfo(bool bResult, const FString& ErrorMessage, const FServersData& Data) override;

public:

	UHubServerDataBaseComponent();

	virtual void CreateServerInDataBase() override;
	virtual void RemoveServerFromDataBase() override;
	virtual bool UpdateServerData() override;

};
