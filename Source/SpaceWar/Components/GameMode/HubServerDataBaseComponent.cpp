// Fill out your copyright notice in the Description page of Project Settings.

#include "HubServerDataBaseComponent.h"
#include "GameFramework/GameState.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"
#include "SpaceWar/Interfaces/DedicatedServerInterfaces.h"
#include "SpaceWar/Objects/Web/WebRequest/WebRequestCreateHubServer.h"
#include "SpaceWar/Objects/Web/WebRequest/WebRequestRemoveHubServerFromDB.h"
#include "SpaceWar/Objects/Web/WebRequest/WebRequestGetServerInfo.h"
#include "SpaceWar/Structs/RemoveServerCallBack.h"

UHubServerDataBaseComponent::UHubServerDataBaseComponent()
{

}

void UHubServerDataBaseComponent::CreateServerInDataBase()
{
	Super::CreateServerInDataBase();
	
	FCreateServerDelegate CallBack;
	CallBack.BindUFunction(this, "OnResponseCreateServer");

	auto const Request = NewObject<UWebRequestCreateHubServer>(GetOwner());
	Request->AddCreateHudServerKey(ServerData.Name, ServerData.Address, CallBack);
	Request->CollectRequest("127.0.0.1/SpaceWar/CreateHudServer.php");
}

void UHubServerDataBaseComponent::RemoveServerFromDataBase()
{
	Super::RemoveServerFromDataBase();
	
	FRemoveServerDelegate CallBack;
	CallBack.BindUFunction(this, "OnResponseRemoveServerFromDataBase");

	auto const Request = NewObject<UWebRequestRemoveHubServerFromDB>(GetOwner());
	Request->AddRemoveServerKey(ServerData.Id, CallBack);
	Request->CollectRequest("127.0.0.1/SpaceWar/RemoveHudServerFromDataBase.php");
}

bool UHubServerDataBaseComponent::UpdateServerData()
{
	if(Super::UpdateServerData())
	{
		auto const Request = NewObject<UWebRequestGetServerInfo>(GetOwner());
		Request->AddServerInfoKey(ServerData.Id, GetServerInfoCallBack);
		Request->CollectRequest("127.0.0.1/SpaceWar/GetHubServerInfo.php");
		return true;
	}
	return false;
}

void UHubServerDataBaseComponent::OnResponseGetServerInfo(bool bResult, const FString& ErrorMessage, const FServersData& Data)
{
	Super::OnResponseGetServerInfo(bResult, ErrorMessage, Data);

	if(!bResult)
	{
		ForcedShutdownServer();
		return;
	} 

	if(!Data.IsActive && ServerData.IsActive)
	{
		DeactivateServer();
	}
	else if(Data.IsActive && !ServerData.IsActive)
	{
		ActivateServer();
	}
	ServerData.IsActive = Data.IsActive;
}

void UHubServerDataBaseComponent::DeactivateServer()
{
	UE_LOG(LogTemp, Warning, TEXT("Server deactivated"));
	OnServerDeactivate.Broadcast();
	StopServerDataTimer();
	StartServerDataTimer(DeactivateTickTime);
}

void UHubServerDataBaseComponent::ActivateServer()
{
	UE_LOG(LogTemp, Warning, TEXT("Server activated"));
	StopServerDataTimer();
	StartServerDataTimer(ActivateTickTime);
}
