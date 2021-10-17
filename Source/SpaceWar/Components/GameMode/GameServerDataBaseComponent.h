// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpaceWar/Structs/CreateServerCallBack.h"
#include "SpaceWar/Structs/ServerInfo.h"
#include "GameServerDataBaseComponent.generated.h"

class AMatchGameModeBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FForcedServerShutdown);

UCLASS( ClassGroup=(Custom), Abstract, meta=(BlueprintSpawnableComponent) )
class SPACEWAR_API UGameServerDataBaseComponent : public UActorComponent
{
	GENERATED_BODY()

	UFUNCTION()
	void OnResponseServerAddress(const FString& Address, const FString& ErrorMessage);

	UFUNCTION()
	void OnResponseGetServerInfo(bool bResult, const FString& ErrorMessage, const FServersData& Data);

	UFUNCTION()
	void ForcedShutdownServer();

public:	
	// Sets default values for this component's properties
	UGameServerDataBaseComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void CallGameServer(const FGameAddressCallBack& CallBack);
	virtual void CreateServerInDataBase() {}
	
	/** removes the server from the database when it is turned off */
	UFUNCTION()
	virtual void RemoveServerFromDataBase();

	UFUNCTION()
	void ShutDownServer();
	
protected:
	
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual bool UpdateServerData();

	UFUNCTION()
    void OnResponseCreateServer(const int32 ServerID);
	
	UFUNCTION()
    void OnResponseRemoveServerFromDataBase(bool bResult, const FString& ErrorMessage);
	
public:

	UPROPERTY()
	FForcedServerShutdown OnForcedServerShutdown;

protected:

	/** Save data about server */
	FServersData ServerData;

	/**
	 *	true if request for get server info send and awaiting a response
	 *	Prevents multiple sent requests that were not answered at the same time.
	*/
	bool bRequestForUpdateSent;

	FGetServerInfoDelegate GetServerInfoCallBack;

friend AMatchGameModeBase;
};
