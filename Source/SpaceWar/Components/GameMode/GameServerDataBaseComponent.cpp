// Fill out your copyright notice in the Description page of Project Settings.

#include "GameServerDataBaseComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "SpaceWar/Objects/Web/WebRequest/WebRequestCreateGameServer.h"
#include "SpaceWar/Objects/Web/WebRequest/WebRequestGetServerAddress.h"
#include "SpaceWar/Objects/Web/WebRequest/WebRequestGetServerInfo.h"
#include "SpaceWar/Objects/Web/WebRequest/WebRequestRemoveServer.h"
#include "SpaceWar/GameModes/Match/Base/MatchGameModeBase.h"
#include "SpaceWar/Structs/CreateServerCallBack.h"
#include "SpaceWar/Structs/RemoveServerCallBack.h"

// Sets default values for this component's properties
UGameServerDataBaseComponent::UGameServerDataBaseComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 10.f; 
	
	bRequestForUpdateSent = false;
}

void UGameServerDataBaseComponent::BeginPlay()
{
	Super::BeginPlay();
	
	GetServerInfoCallBack.BindUFunction(this, "OnResponseGetServerInfo");
	
	auto const GM = UGameplayStatics::GetGameMode(GetWorld());
	if(!GM) return;

	/** Shut down server if server have ot option name */
	if(!UGameplayStatics::HasOption(GM->OptionsString, "ServerName"))
	{
		UE_LOG(LogGameMode, Error, TEXT("Server have not ServerName. Add Server name option and launch server"));
		ShutDownServer();
		return;
	}
	
	ServerData.Name = UGameplayStatics::ParseOption(GM->OptionsString, "ServerName");
	ServerData.MapName = GetWorld()->GetMapName();
	ServerData.MapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	
	if(UGameplayStatics::HasOption(GM->OptionsString, "ip"))
	{
		ServerData.Address = UGameplayStatics::ParseOption(GM->OptionsString, "ip");
		ServerData.Address += ":" + FString::FromInt(GetWorld()->URL.Port);
		CreateServerInDataBase();
	}
	else
	{
		FGameAddressCallBack CallBack;
		CallBack.OnGameServerAddress.BindUFunction(this, "OnResponseServerAddress");
		CallGameServer(CallBack);
	}
}

void UGameServerDataBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(!GetOwner()) return;
	UpdateServerData();
}

void UGameServerDataBaseComponent::UpdateServerData()
{
	if(!ServerData.IsActive || bRequestForUpdateSent) return;

	bRequestForUpdateSent = true;

	auto const Request = NewObject<UWebRequestGetServerInfo>(GetOwner());
	Request->AddServerInfoKey(ServerData.Id, GetServerInfoCallBack);
	Request->CollectRequest("127.0.0.1/SpaceWar/GetServerInfo.php");
}

void UGameServerDataBaseComponent::OnResponseServerAddress(const FString& Address, const FString& ErrorMessage)
{
	UE_LOG(LogTemp, Warning, TEXT("OnResponseServerAddress --Server address: %s"), *Address);
	ServerData.Address = Address;
	ServerData.Address += ":" + FString::FromInt(GetWorld()->URL.Port);
	CreateServerInDataBase();
}

/** Create Server */
void UGameServerDataBaseComponent::CreateServerInDataBase()
{
	FCreateServerDelegate CallBack;
	CallBack.BindUFunction(this, "OnResponseCreateServer");

	auto const Request = NewObject<UWebRequestCreateGameServer>(GetOwner());
	Request->AddCreateServerKeys(ServerData.Name, ServerData.Address, ServerData.MapName, CallBack);
	Request->CollectRequest("127.0.0.1/SpaceWar/CreateGameServer.php");
}

void UGameServerDataBaseComponent::OnResponseCreateServer(const int32 ServerID)
{
	if(ServerID < 0)
	{
		ShutDownServer();
		return;
	}
	ServerData.Id = ServerID;
	ServerData.IsActive = true;
}

void UGameServerDataBaseComponent::OnResponseGetServerInfo(bool bResult, const FString& ErrorMessage, const FServersData& Data)
{
	UE_LOG(LogTemp, Log, TEXT("OnResponseGetServerInfo. Name: %s     Address: %s"), *ServerData.Name, *ServerData.Address);
	if(!bResult) return;
	
	bRequestForUpdateSent = false;
	ServerData.Name = Data.Name;
	ServerData.IsActive = Data.IsActive;

	if(!Data.IsActive)
	{
		ForcedShutdownServer();
	}
}

/** Remove server */
void UGameServerDataBaseComponent::RemoveServerFromDataBase()
{
	FRemoveServerDelegate CallBack;
	CallBack.BindUFunction(this, "OnResponseRemoveServerFromDataBase");

	ServerData.IsActive = false;
	SetTickableWhenPaused(true);
	
	auto const Request = NewObject<UWebRequestRemoveServer>(GetOwner());
	Request->AddRemoveServerKey(ServerData.Id, CallBack);
	Request->CollectRequest("127.0.0.1/SpaceWar/RemoveServerFromDataBase.php");
}

void UGameServerDataBaseComponent::OnResponseRemoveServerFromDataBase(bool bResult, const FString& ErrorMessage)
{
	if(bResult)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnResponseRemoveServerFromDataBase"));
	}
}

void UGameServerDataBaseComponent::CallGameServer(const FGameAddressCallBack& CallBack)
{
	auto const Request = NewObject<UWebRequestGetServerAddress>(GetOwner());
	Request->AddServerAddressKey(CallBack.OnGameServerAddress);
	Request->CollectRequest("127.0.0.1/SpaceWar/GetServerAddress.php");
}

void UGameServerDataBaseComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UGameServerDataBaseComponent::ShutDownServer()
{
	ServerData.IsActive = false;
	RequestEngineExit("Game over");
}

void UGameServerDataBaseComponent::ForcedShutdownServer()
{
	OnForcedServerShutdown.Broadcast();
	SetTickableWhenPaused(true);
	ServerData.IsActive = false;
}
