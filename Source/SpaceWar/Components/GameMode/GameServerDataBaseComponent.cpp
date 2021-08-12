// Fill out your copyright notice in the Description page of Project Settings.

#include "GameServerDataBaseComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "SpaceWar/Objects/Web/WebRequest/WebRequestCreateGameServer.h"
#include "SpaceWar/Objects/Web/WebRequest/WebRequestGetServerAddress.h"
#include "SpaceWar/Objects/Web/WebRequest/WebRequestGetServerInfo.h"
#include "SpaceWar/Objects/Web/WebRequest/WebRequestRemoveServer.h"
#include "SpaceWar/Structs/CreateServerCallBack.h"
#include "SpaceWar/Structs/RemoveServerCallBack.h"

// Sets default values for this component's properties
UGameServerDataBaseComponent::UGameServerDataBaseComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 2.f; 
	
	bServerActive = false;
	bRequestForUpdateSent = false;
}

void UGameServerDataBaseComponent::BeginPlay()
{
	Super::BeginPlay();

	auto const GM = UGameplayStatics::GetGameMode(GetWorld());
	if(!GM) return;
	
	ServerData.Name = UGameplayStatics::ParseOption(GM->OptionsString, "ServerName");

	/*LevelName = GetWorld()->GetMapName();
	LevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);*/
	
	FGameAddressCallBack CallBack;
	CallBack.OnGameServerAddress.BindUFunction(this, "OnResponseServerAddress");
	CallGameServer(CallBack);

	/** test */
	FTimerDelegate TimerDel;
	FTimerHandle TimerHandle;
	auto f = [&]() -> void
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		RemoveServerFromDataBase();
		UE_LOG(LogTemp, Warning, TEXT("------------------------------------------"));
		UE_LOG(LogTemp, Warning, TEXT("Remove server from database"));
		UE_LOG(LogTemp, Warning, TEXT("------------------------------------------"));
	};
	TimerDel.BindLambda(f);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, 10.f, false);
}

void UGameServerDataBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateServerData();
}

void UGameServerDataBaseComponent::UpdateServerData()
{
	if(!bServerActive || bRequestForUpdateSent) return;

	bRequestForUpdateSent = true;
	
	FGetServerInfoDelegate CallBack;
	CallBack.BindUFunction(this, "OnResponseGetServerInfo");

	auto const Request = NewObject<UWebRequestGetServerInfo>(GetOwner());
	Request->AddServerInfoKey(ServerData.Id, CallBack);
	Request->CollectRequest("127.0.0.1/SpaceWar/GetServerInfo.php");
}

void UGameServerDataBaseComponent::OnResponseServerAddress(const FString& Address, const FString& ErrorMessage)
{
	UE_LOG(LogTemp, Warning, TEXT("OnResponseServerAddress --Server address: %s"), *Address);
	ServerData.Address = Address;
	CreateServerInDataBase();
}

/** Create Server */
void UGameServerDataBaseComponent::CreateServerInDataBase()
{
	FCreateServerDelegate CallBack;
	CallBack.BindUFunction(this, "OnResponseCreateServer");

	auto const Request = NewObject<UWebRequestCreateGameServer>(GetOwner());
	Request->AddCreateServerKeys(ServerData.Name, ServerData.Address, CallBack);
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
	bServerActive = true;
}

void UGameServerDataBaseComponent::OnResponseGetServerInfo(bool bResult, const FString& ErrorMessage, const FServersData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("OnResponseGetServerInfo. Name: %s     Address: %s"), *ServerData.Name, *ServerData.Address);
	if(!bResult) return;
	
	bRequestForUpdateSent = false;
	ServerData.Address = Data.Address;
	ServerData.Name = Data.Name;
}

/** Remove server */
void UGameServerDataBaseComponent::RemoveServerFromDataBase()
{
	FRemoveServerDelegate CallBack;
	CallBack.BindUFunction(this, "OnResponseRemoveServerFromDataBase");

	bServerActive = false;
	SetTickableWhenPaused(true);
	
	auto const Request = NewObject<UWebRequestRemoveServer>(GetOwner());
	Request->AddRemoveServerKey(ServerData.Id, CallBack);
	Request->CollectRequest("127.0.0.1/SpaceWar/RemoveServerFromDataBase.php");
}

void UGameServerDataBaseComponent::OnResponseRemoveServerFromDataBase(bool bResult, const FString& ErrorMessage)
{
	ShutDownServer();
	UE_LOG(LogTemp, Warning, TEXT("OnResponseRemoveServerFromDataBase"));
}

void UGameServerDataBaseComponent::CallGameServer(const FGameAddressCallBack& CallBack)
{
	auto const Request = NewObject<UWebRequestGetServerAddress>(GetOwner());
	Request->AddServerAddressKey(CallBack.OnGameServerAddress);
	Request->CollectRequest("127.0.0.1/SpaceWar/GetServerAddress.php");
}

void UGameServerDataBaseComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RemoveServerFromDataBase();
	
	Super::EndPlay(EndPlayReason);
}

void UGameServerDataBaseComponent::ShutDownServer()
{
	bServerActive = false;
	RequestEngineExit("");
}