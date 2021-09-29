// Fill out your copyright notice in the Description page of Project Settings.


#include "OnlinePlayerStateBase.h"
#include "Net/UnrealNetwork.h"
#include "SpaceWar/GameInstances/BaseGameInstance.h"

AOnlinePlayerStateBase::AOnlinePlayerStateBase()
{
	bUseCustomPlayerNames = true;
}

void AOnlinePlayerStateBase::BeginPlay()
{
	Super::BeginPlay();

/*	if(GetLocalRole() != ROLE_Authority)
	{
		auto const GameInstance = GetGameInstance<UBaseGameInstance>();
		Server_TransferPlayerNameToServer(GameInstance->GetPlayerName());
	}*/
}

void AOnlinePlayerStateBase::SetTeam(const ETeam NewTeam)
{
	PlayerTeam = NewTeam;
}

void AOnlinePlayerStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AOnlinePlayerStateBase, PlayerTeam);
	DOREPLIFETIME(AOnlinePlayerStateBase, NumberOfDeaths);
	DOREPLIFETIME(AOnlinePlayerStateBase, NumberOfMurders);
}

void AOnlinePlayerStateBase::IncrementNumberOfDeaths()
{
	NumberOfDeaths++;
	OnRep_NumberOfDeaths();
}

void AOnlinePlayerStateBase::DecrementNumberOfMurders()
{
	NumberOfMurders--;
	OnNumberOnMurdersDecrease.Broadcast();
	OnRep_NumberOfMurders();
}

void AOnlinePlayerStateBase::IncrementNumberOfMurders()
{
	NumberOfMurders++;
	OnRep_NumberOfMurders();
}

void AOnlinePlayerStateBase::OnRep_NumberOfDeaths()
{
	OnNumberOfDeathsChanged.Broadcast();
}

void AOnlinePlayerStateBase::OnRep_NumberOfMurders()
{
	OnNumberOfMurdersChanged.Broadcast();
}

void AOnlinePlayerStateBase::Server_TransferPlayerNameToServer_Implementation(const FString& Name)
{
	SetPlayerName(Name);
}



