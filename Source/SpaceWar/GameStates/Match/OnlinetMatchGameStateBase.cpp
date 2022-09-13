// Fill out your copyright notice in the Description page of Project Settings.


#include "OnlinetMatchGameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "SpaceWar/GameModes/Match/OnlineMatchGameModeBase.h"
#include "SpaceWar/Interfaces/PlayerControllerInterface.h"

AOnlinetMatchGameStateBase::AOnlinetMatchGameStateBase()
{
	TeamPointsA = 0;
	TeamPointsB = 0;
	MaxGamePreparationTime = 20;

	VotingDatas.Reserve(2);
	VotingDatas[0] = FVotingData();
	VotingDatas[1] = FVotingData();
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
	int32 PlayerForStartGame = 2;
#if UE_EDITOR
	CurrentMatchTime = 2.f;
	PlayerForStartGame = 1;
#endif
	
	if(PlayerArray.Num() >= PlayerForStartGame && !bGameInProgress && !GetWorld()->GetTimerManager().IsTimerActive(PreparationGameStartHandle))
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
		return;
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
#if UE_SERVER
	if(PlayerArray.Num() < 2 && !bGameInProgress)
	{
		FinishPreparationGame(false);
		GetWorld()->GetTimerManager().ClearTimer(PreparationGameStartHandle);
	}
#endif
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

FVotingData* AOnlinetMatchGameStateBase::GetVotingDataByTeam(ETeam const Team)
{
	return &VotingDatas[(uint8)Team - 1];
}

void AOnlinetMatchGameStateBase::StartVotingForKickPlayer_Implementation(APlayerController* KickPlayer, ETeam const VotingOwnerTeam)
{
	auto VotingData = GetVotingDataByTeam(VotingOwnerTeam);
	if(!VotingData) return;
	if(GetWorld()->GetTimerManager().IsTimerActive(VotingData->VotingTimer) && !KickPlayer) return;

	for(auto const ByArray : PlayerArray)
	{
		if(VotingOwnerTeam == IGetPlayerTeamInterface::Execute_FindPlayerTeam(ByArray))
		{
			if(ByArray->GetOwner() == KickPlayer) continue;
			IPlayerControllerInterface::Execute_VotingStartedForKickPlayer(ByArray->GetOwner());
		}
	}
	VotingData->KickPlayer = KickPlayer;
	FTimerDelegate TimerDel;
	TimerDel.BindUObject(this, &AOnlinetMatchGameStateBase::StopVoting, VotingData);
	GetWorld()->GetTimerManager().SetTimer(VotingData->VotingTimer, TimerDel, 30.f, false);
}

void AOnlinetMatchGameStateBase::StopVoting(FVotingData* VotingData)
{
	GetWorld()->GetTimerManager().ClearTimer(VotingData->VotingTimer);

	if(VotingData->AmountForKick > VotingData->AmountForSave)
	{
		VotingData->KickPlayer->ClientWasKicked(FText(FString("Player be kicked")));
	}
	VotingData->KickPlayer = nullptr;
	VotingData->AmountForKick = 0;
	VotingData->AmountForSave = 0;
}

