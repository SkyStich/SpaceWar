// Fill out your copyright notice in the Description page of Project Settings.


#include "GameServerDataBaseComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "SpaceWar/Objects/Web/WebRequest/WebRequestCreateGameServer.h"
#include "SpaceWar/Objects/Web/WebRequest/WebRequestGetServerAddress.h"
#include "SpaceWar/Structs/CreateServerCallBack.h"

// Sets default values for this component's properties
UGameServerDataBaseComponent::UGameServerDataBaseComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
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

void UGameServerDataBaseComponent::OnResponseCreateServer(bool bResult)
{
	
}

void UGameServerDataBaseComponent::OnResponseServerAddress(const FString& Address)
{
	UE_LOG(LogTemp, Warning, TEXT("OnResponseServerAddress --Server address: %s"), *Address);
	//CreateServerInDataBase(Address);
}

void UGameServerDataBaseComponent::CreateServerInDataBase(const FString& Address)
{
	FCreateServerDelegate CallBack;
	CallBack.BindUFunction(this, "OnResponseCreateServer");

	auto const Response = NewObject<UWebRequestCreateGameServer>(GetOwner());
	Response->AddCreateServerKeys(LevelName, Address, CallBack);
	Response->CollectRequest("127.0.0.1/CreateGameServer.php");
	UE_LOG(LogTemp, Warning, TEXT("CreateServerInDataBase"));
}

void UGameServerDataBaseComponent::RemoveServerFromDataBase()
{
	
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

