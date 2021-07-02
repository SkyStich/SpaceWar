// Fill out your copyright notice in the Description page of Project Settings.


#include "OnlinetMatchGameStateBase.h"
#include "Net/UnrealNetwork.h"

AOnlinetMatchGameStateBase::AOnlinetMatchGameStateBase()
{
	TeamPointsA = 0;
	TeamPointsB = 0;
}

void AOnlinetMatchGameStateBase::NewPlayerLogin(APlayerController* PC)
{
	Super::NewPlayerLogin(PC);

	SetTeamForPlayer(PC);
	
	NetMulticast_NewPlayerPostLogin(PC->PlayerState);
}

void AOnlinetMatchGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AOnlinetMatchGameStateBase, TeamPointsA);
	DOREPLIFETIME(AOnlinetMatchGameStateBase, TeamPointsB);
}


void AOnlinetMatchGameStateBase::SetTeamForPlayer(APlayerController* PC)
{
	auto const OnlinePlayerState = Cast<AOnlinePlayerStateBase>(PC->PlayerState);
	if(!OnlinePlayerState) return;

	if((PlayerArray.Num() % 2) == 1)
	{
		OnlinePlayerState->SetTeam(ETeam::TeamA);
	}
	else
	{
		OnlinePlayerState->SetTeam(ETeam::TeamB);	
	}
}

void AOnlinetMatchGameStateBase::AutoBalanceTeam()
{
	
}

void AOnlinetMatchGameStateBase::UpdateTeamPoints(ETeam Team, int32 Value)
{
	if(GetLocalRole() != ROLE_Authority) return;
	
	if(Team == ETeam::TeamA)
	{
		TeamPointsA += Value;
		OnTeamPointUpdate.Broadcast(TeamPointsA, Team);
		return;
	}
	TeamPointsB += Value;
	OnTeamPointUpdate.Broadcast(TeamPointsB, Team);
}

void AOnlinetMatchGameStateBase::NetMulticast_NewPlayerPostLogin_Implementation(APlayerState* PlayerState)
{
	OnNewPlayerPostLogin.Broadcast(PlayerState);
}



