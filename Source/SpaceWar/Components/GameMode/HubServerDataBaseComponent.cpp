// Fill out your copyright notice in the Description page of Project Settings.

#include "HubServerDataBaseComponent.h"
#include "SpaceWar/Objects/Web/WebRequest/WebRequestCreateHubServer.h"
#include "SpaceWar/Objects/Web/WebRequest/WebRequestRemoveHubServerFromDB.h"
#include "SpaceWar/Objects/Web/WebRequest/WebRequestGetServerInfo.h"
#include "SpaceWar/Structs/RemoveServerCallBack.h"

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

