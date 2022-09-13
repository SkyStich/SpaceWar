// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpaceWar/Structs/CreateServerCallBack.h"
#include "SpaceWar/Structs/ServerInfo.h"
#include "GameServerDataBaseComponent.generated.h"

class AMatchGameModeBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FForcedServerShutdown);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FServerDeactivate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEWAR_API UGameServerDataBaseComponent : public UActorComponent
{
	GENERATED_BODY()

	UFUNCTION()
	void OnResponseServerAddress(const FString& Address, const FString& ErrorMessage);

	UFUNCTION()
	void OnUpdateServerData();

public:	
	// Sets default values for this component's properties
	UGameServerDataBaseComponent();
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void CallGameServer(const FGameAddressCallBack& CallBack);
	virtual void CreateServerInDataBase() {}

	FServersData GetServerData() const { return ServerData; }
	
	/** removes the server from the database when it is turned off */
	UFUNCTION()
	virtual void RemoveServerFromDataBase();

	UFUNCTION()
	void ShutDownServer(const FString& Reason);

	UFUNCTION()
	void ForcedShutdownServer();
	
protected:
	
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual bool UpdateServerData();
	virtual void StartServerDataTimer(const float TickRate);
	void StopServerDataTimer() { GetWorld()->GetTimerManager().ClearTimer(ServerDataHandle); }
	
	UFUNCTION()
	virtual void OnResponseGetServerInfo(bool bResult, const FString& ErrorMessage, const FServersData& Data);

	UFUNCTION()
    void OnResponseCreateServer(const int32 ServerID);
	
	UFUNCTION()
    void OnResponseRemoveServerFromDataBase(bool bResult, const FString& ErrorMessage);
	
public:

	UPROPERTY(BlueprintAssignable)
	FForcedServerShutdown OnForcedServerShutdown;

	UPROPERTY(BlueprintAssignable)
	FServerDeactivate OnServerDeactivate;

protected:

	/** Save data about server */
	FServersData ServerData;

	/**
	 *	true if request for get server info send and awaiting a response
	 *	Prevents multiple sent requests that were not answered at the same time.
	*/
	bool bRequestForUpdateSent;

	float ActivateTickTime;
	float DeactivateTickTime;

	FGetServerInfoDelegate GetServerInfoCallBack;
private:

	/** Need to update server data from database */
	UPROPERTY()
	FTimerHandle ServerDataHandle;
};
