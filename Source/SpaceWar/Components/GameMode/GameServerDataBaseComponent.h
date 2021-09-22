// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpaceWar/Structs/CreateServerCallBack.h"
#include "SpaceWar/Structs/ServerInfo.h"


#include "GameServerDataBaseComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEWAR_API UGameServerDataBaseComponent : public UActorComponent
{
	GENERATED_BODY()

	UFUNCTION()
	void OnResponseServerAddress(const FString& Address, const FString& ErrorMessage);

	UFUNCTION()
	void OnResponseCreateServer(const int32 ServerID);

	UFUNCTION()
	void OnResponseGetServerInfo(bool bResult, const FString& ErrorMessage, const FServersData& Data);

	UFUNCTION()
	void OnResponseRemoveServerFromDataBase(bool bResult, const FString& ErrorMessage);

	void UpdateServerData();

public:	
	// Sets default values for this component's properties
	UGameServerDataBaseComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void CallGameServer(const FGameAddressCallBack& CallBack);
	void CreateServerInDataBase();
	
	/** removes the server from the database when it is turned off */
	UFUNCTION()
	void RemoveServerFromDataBase();

	UFUNCTION()
	void ShutDownServer();
	
protected:
	
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	/** Save data about server */
	FServersData ServerData;

	/** true if server created in data base in activated */
	bool bServerActive;

	/**
	 *	true if request for get server info send and awaiting a response
	 *	Prevents multiple sent requests that were not answered at the same time.
	*/
	bool bRequestForUpdateSent;

	FGetServerInfoDelegate GetServerInfoCallBack;
};
