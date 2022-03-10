// Fill out your copyright notice in the Description page of Project Settings.


#include "GameDataBaseComponent.h"
#include "GameFramework/GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "SpaceWar/Objects/Web/WebRequest/WebRequestCreateGameServer.h"
#include "SpaceWar/Objects/Web/WebRequest/WebRequestGetServerInfo.h"
#include "SpaceWar/Objects/Web/WebRequest/WebRequestRemoveServer.h"
#include "SpaceWar/Structs/RemoveServerCallBack.h"

void UGameDataBaseComponent::CreateServerInDataBase()
{
	Super::CreateServerInDataBase();

#if UE_SERVER
	auto const GameMode = UGameplayStatics::GetGameMode(GetWorld());
	if(!GameMode) return;
	if(UGameplayStatics::HasOption(GameMode->OptionsString, "OwnerName"))
	{
		FString OwnerName = UGameplayStatics::ParseOption(GameMode->OptionsString, "OwnerName");
		FCreateServerDelegate CallBack;
		CallBack.BindUFunction(this, "OnResponseCreateServer");

		auto const Request = NewObject<UWebRequestCreateGameServer>(GetOwner());
		Request->AddCreateServerKeys(ServerData.Name, ServerData.Address, ServerData.MapName, OwnerName, CallBack);
		Request->CollectRequest("127.0.0.1/SpaceWar/CreateGameServer.php");
		return;
	}
	UE_LOG(LogActorComponent, Error, TEXT("Game mode have not option OwnerName (Server hub owner)"));
#endif
}

void UGameDataBaseComponent::RemoveServerFromDataBase()
{
	Super::RemoveServerFromDataBase();

	FRemoveServerDelegate CallBack;
	CallBack.BindUFunction(this, "OnResponseRemoveServerFromDataBase");
	
	auto const Request = NewObject<UWebRequestRemoveServer>(GetOwner());
	Request->AddRemoveServerKey(ServerData.Id, CallBack);
	Request->CollectRequest("127.0.0.1/SpaceWar/RemoveServerFromDataBase.php");
}

bool UGameDataBaseComponent::UpdateServerData()
{
	if(Super::UpdateServerData())
	{
		auto const Request = NewObject<UWebRequestGetServerInfo>(GetOwner());
		Request->AddServerInfoKey(ServerData.Id, GetServerInfoCallBack);
		Request->CollectRequest("127.0.0.1/SpaceWar/GetGameServerInfo.php");
		return true;
	}
	return false;
}

