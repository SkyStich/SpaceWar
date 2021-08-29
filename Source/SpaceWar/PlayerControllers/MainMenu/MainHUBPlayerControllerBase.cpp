// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUBPlayerControllerBase.h"
#include "Kismet/GameplayStatics.h"
#include "SpaceWar/BPFLibrary/ServerManipulationLibrary.h"

void AMainHUBPlayerControllerBase::Server_CreateServer_Implementation(const FString& MapReference, const FString& MapName, const FString& ServerName)
{
	UServerManipulationLibrary::LaunchServer(MapReference, FString::Printf(TEXT("ip=127.0.0.1 ServerName=%s"), *ServerName));
}

void AMainHUBPlayerControllerBase::CreateServer(const FMapData& Data, const FString& ServerName)
{
	Server_CreateServer(Data.MapReference, Data.MapName, ServerName);

	CreateData = Data;
	
	FTimerHandle SendRequestHandle;
	FTimerDelegate TimerDel;
	TimerDel.BindUObject(this, &AMainHUBPlayerControllerBase::CheckingForExistenceServer);
	GetWorld()->GetTimerManager().SetTimer(SendRequestHandle, TimerDel, 1.f, true, 2.f);
}

void AMainHUBPlayerControllerBase::CheckingForExistenceServer()
{
	if(bRequestSent) return;

	bRequestSent = true;
	OnCreateServerComplete.BindUFunction(this, "OnCreteServerResult");
	ClientServerTransfer->RequestReceivingCreateServerResult(OnCreateServerComplete);
}

void AMainHUBPlayerControllerBase::OnCreteServerResult(bool bResult, const FString& Address)
{
	bRequestSent = false;
	if(bResult)
	{
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
		UGameplayStatics::OpenLevel(GetWorld(), *Address);
	}
}

