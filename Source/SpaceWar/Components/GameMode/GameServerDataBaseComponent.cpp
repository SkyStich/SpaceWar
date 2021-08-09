// Fill out your copyright notice in the Description page of Project Settings.


#include "GameServerDataBaseComponent.h"



#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "SpaceWar/Objects/Web/WebRequest/WebRequestCreateGameServer.h"
#include "SpaceWar/Structs/CreateServerCallBack.h"

// Sets default values for this component's properties
UGameServerDataBaseComponent::UGameServerDataBaseComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UGameServerDataBaseComponent::BeginPlay()
{
	Super::BeginPlay();

	auto const GM = UGameplayStatics::GetGameMode(GetWorld());
	if(!GM) return;
	ServerName = UGameplayStatics::ParseOption(GM->OptionsString, "ServerName");

	LevelName = GetWorld()->GetMapName();
	LevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	
	FGameAddressCallBack CallBack;
	CallBack.OnGameServerAddress.BindUFunction(this, "OnResponseServerAddress");
	CallGameServer(CallBack);
}

void UGameServerDataBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UGameServerDataBaseComponent::OnResponseCreateServer()
{
	
}

void UGameServerDataBaseComponent::OnResponseServerAddress(const FString& Address)
{
	CreateServerInDataBase(Address);
}

void UGameServerDataBaseComponent::CreateServerInDataBase(const FString& Address)
{
	FCreateServerDelegate CallBack;
	CallBack.BindUFunction(this, "OnResponseCreateServer");

	auto const Response = NewObject<UWebRequestCreateGameServer>(GetOwner());
	Response->AddCreateServerKeys(FCreateServerData("Name",ServerName, Address), CallBack);
	Response->CollectRequest("127.0.0.1/CreateGameServer.php");
}

void UGameServerDataBaseComponent::RemoveServerFromDataBase()
{
	
}


void UGameServerDataBaseComponent::CallGameServer(const FGameAddressCallBack& CallBack)
{
	GameAddressCallBack = CallBack;
}

void UGameServerDataBaseComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RemoveServerFromDataBase();
	
	Super::EndPlay(EndPlayReason);
}

