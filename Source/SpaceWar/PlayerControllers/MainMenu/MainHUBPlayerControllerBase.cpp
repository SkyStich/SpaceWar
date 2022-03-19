// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUBPlayerControllerBase.h"

#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "SpaceWar/BPFLibrary/ServerManipulationLibrary.h"
#include "SpaceWar/Components/GameMode/GameServerDataBaseComponent.h"
#include "SpaceWar/GameInstances/BaseGameInstance.h"

void AMainHUBPlayerControllerBase::Server_CreateServer_Implementation(const FString& MapReference, const FString& ServerName)
{
	UE_LOG(LogTemp, Log, TEXT("Reference: %s"), *MapReference)
	auto const GameModeComponent = UGameplayStatics::GetGameMode(GetWorld())->FindComponentByClass<UGameServerDataBaseComponent>();
	if(GameModeComponent)
	{
		FString Param = TEXT("ServerName=") + ServerName + "?OwnerName=" + GameModeComponent->GetServerData().Name + "?ip=26.217.133.211 ";
		UServerManipulationLibrary::LaunchServer(MapReference, Param);
	}
}

void AMainHUBPlayerControllerBase::CreateServer(const FMapData& Data, FString ServerName)
{
	Server_CreateServer(Data.MapReference, ServerName);

	CreateData = Data;

	/** send timer for chack server on created */
	FTimerHandle SendRequestHandle;
	FTimerDelegate TimerDel;
	TimerDel.BindUObject(this, &AMainHUBPlayerControllerBase::CheckingForExistenceServer, ServerName);
	GetWorld()->GetTimerManager().SetTimer(SendRequestHandle, TimerDel, 2.f, true, 3.f);
}

void AMainHUBPlayerControllerBase::CheckingForExistenceServer(FString ServerName)
{
	if(bRequestSent) return;

	bRequestSent = true;
	OnCreateServerComplete.BindUFunction(this, "OnCreteServerResult");
	ClientServerTransfer->RequestReceivingCreateServerResult(ServerName, OnCreateServerComplete);
}

void AMainHUBPlayerControllerBase::OnCreteServerResult(bool bResult, const FString& Address)
{
	UE_LOG(LogTemp, Error, TEXT("OnCreteServerResult: "), bResult);
	bRequestSent = false;
	if(bResult)
	{
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
		UGameplayStatics::OpenLevel(GetWorld(), *Address);
	}
}

