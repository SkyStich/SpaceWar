// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUBPlayerControllerBase.h"
#include "Kismet/GameplayStatics.h"
#include "SpaceWar/BPFLibrary/ServerManipulationLibrary.h"

void AMainHUBPlayerControllerBase::Server_CreateServer_Implementation(const FString& MapReference, const FString& ServerName)
{
	UE_LOG(LogTemp, Log, TEXT("Reference: %s"), *MapReference)
	FString Param = TEXT("ServerName=") + ServerName;
	UServerManipulationLibrary::LaunchServer(MapReference, Param);
}

void AMainHUBPlayerControllerBase::CreateServer(const FMapData& Data, FString ServerName)
{
	Server_CreateServer(Data.MapReference, ServerName);

	CreateData = Data;
	
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
	UE_LOG(LogTemp, Error, TEXT("Result: "), bResult);
	bRequestSent = false;
	if(bResult)
	{
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
		UGameplayStatics::LoadStreamLevel(GetWorld(), *Address, true, false, FLatentActionInfo());
	}
}

