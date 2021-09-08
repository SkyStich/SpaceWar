// Fill out your copyright notice in the Description page of Project Settings.


#include "OnlinetMatchGameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "SpaceWar/GameModes/Match/OnlineMatchGameModeBase.h"

AOnlinetMatchGameStateBase::AOnlinetMatchGameStateBase()
{
	TeamPointsA = 0;
	TeamPointsB = 0;
	MaxGamePreparationTime = 20;
}

void AOnlinetMatchGameStateBase::NewPlayerLogin(APlayerController* PC)
{
	Super::NewPlayerLogin(PC);

	SetTeamForPlayer(PC);
	PreparationForStartGame();
	NetMulticast_NewPlayerPostLogin(PC->PlayerState);
}

void AOnlinetMatchGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AOnlinetMatchGameStateBase, TeamPointsA);
	DOREPLIFETIME(AOnlinetMatchGameStateBase, TeamPointsB);
	DOREPLIFETIME(AOnlinetMatchGameStateBase, bGameInProgress);
}

void AOnlinetMatchGameStateBase::BeginPlay()
{
	Super::BeginPlay();

	if(GetLocalRole() == ROLE_Authority)
	{
		auto GM = Cast<AOnlineMatchGameModeBase>(AuthorityGameMode);
		GM->OnPlayerLogout.AddDynamic(this, &AOnlinetMatchGameStateBase::Logout);
	}
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

void AOnlinetMatchGameStateBase::PreparationForStartGame()
{
	if(PlayerArray.Num() >= 2 && !bGameInProgress && !GetWorld()->GetTimerManager().IsTimerActive(PreparationGameStartHandle))
	{
		auto f = [&]() -> void
		{
			CurrentMatchTime--;
			if(CurrentMatchTime <= 0)
			{
				MatchStarted();
				GetWorld()->GetTimerManager().ClearTimer(PreparationGameStartHandle);
			}
		};
		
		CurrentMatchTime = MaxGamePreparationTime;
		FTimerDelegate TimerDel;
		TimerDel.BindLambda(f);
		GetWorld()->GetTimerManager().SetTimer(PreparationGameStartHandle, TimerDel, 1.f, true);
	}
}

void AOnlinetMatchGameStateBase::MatchStarted()
{
	CurrentMatchTime = MaxMatchTime;
	bGameInProgress = true;
	FinishPreparationGame(true);
}

void AOnlinetMatchGameStateBase::Logout(AController* Exiting)
{
	if(PlayerArray.Num() < 2 && !bGameInProgress)
	{
		FinishPreparationGame(false);
		GetWorld()->GetTimerManager().ClearTimer(PreparationGameStartHandle);
	}
}

void AOnlinetMatchGameStateBase::NetMulticast_NewPlayerPostLogin_Implementation(APlayerState* PlayerState)
{
	OnNewPlayerPostLogin.Broadcast(PlayerState);
}

void AOnlinetMatchGameStateBase::FinishPreparationGame(bool bResult)
{
	NetMulticast_FinishPreparationStartGame(bResult);
}

void AOnlinetMatchGameStateBase::NetMulticast_FinishPreparationStartGame_Implementation(bool bResult)
{
	OnPreparationStartGameFinish.Broadcast(bResult);
}



