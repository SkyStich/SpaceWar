// Fill out your copyright notice in the Description page of Project Settings.


#include "CaptureOfFlagGameState.h"
#include "Net/UnrealNetwork.h"

ACaptureOfFlagGameState::ACaptureOfFlagGameState()
{
	CurrentPreparationTime = 15;
	SecurityTeam = ETeam::TeamA;
}

void ACaptureOfFlagGameState::BeginPlay()
{
	Super::BeginPlay();
}

void ACaptureOfFlagGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACaptureOfFlagGameState, SecurityTeam);
	DOREPLIFETIME(ACaptureOfFlagGameState, CurrentPreparationTime);
}

void ACaptureOfFlagGameState::FinishPreparationGame(bool bResult)
{
	Super::FinishPreparationGame(bResult);

	RefreshRound();
}

void ACaptureOfFlagGameState::RefreshRound()
{
	CurrentPreparationTime = 20;
	SecurityTeam = (GetTeamPointsA() + GetTeamPointsB()) % 2 == 0 ? ETeam::TeamA : ETeam::TeamB;
	NetMulticast_RoundPreparation();
	
	GetWorld()->GetTimerManager().ClearTimer(RefreshRoundHandle);
	GetWorld()->GetTimerManager().SetTimer(RefreshRoundHandle, this, &ACaptureOfFlagGameState::ReductionPreparationTime, 1.f, true);
}

void ACaptureOfFlagGameState::ReductionPreparationTime()
{
	CurrentPreparationTime--;
	if(CurrentPreparationTime <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(RefreshRoundHandle);
		NetMulticast_RoundStarted();
	}
}

void ACaptureOfFlagGameState::MatchFinish_Implementation(const FString& Reason, ETeam WinnerTeam)
{
	Super::MatchFinish_Implementation(Reason, WinnerTeam);

	GetWorld()->GetTimerManager().ClearTimer(RefreshRoundHandle);
}

void ACaptureOfFlagGameState::NetMulticast_RoundEnded_Implementation(const FString& Reason, ETeam WinnerTeam, EReasonForEndOfRound ReasonEndOfRound)
{
	if(GetLocalRole() == ROLE_Authority)
	{
		GetWorld()->GetTimerManager().ClearTimer(RefreshRoundHandle);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("End round"));
		GetWorld()->GetTimerManager().SetTimer(RefreshRoundHandle, this, &ACaptureOfFlagGameState::RefreshRound, 5.f, false);
	}

	OnRoundEnded.Broadcast(Reason, WinnerTeam, ReasonEndOfRound);
}

void ACaptureOfFlagGameState::NetMulticast_RoundStarted_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Start round"));
	OnRoundStarted.Broadcast();
}

void ACaptureOfFlagGameState::NetMulticast_RoundPreparation_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("preparaction round"));
	if(GetLocalRole() == ROLE_Authority)
	{
		CurrentMatchTime = MaxMatchTime;
	}
	OnRoundPreparation.Broadcast();
}

void ACaptureOfFlagGameState::UpdateTeamPoints(ETeam Team, int32 Value, EReasonForEndOfRound ReasonEndOfRound)
{
	if(Team == ETeam::TeamA)
	{
		TeamPointsA += Value;
		OnTeamPointUpdate.Broadcast(TeamPointsA, Team);
	}
	else
	{
		TeamPointsB += Value;
	}
	NetMulticast_RoundEnded(UEnum::GetValueAsString(ReasonEndOfRound), Team, ReasonEndOfRound);
	OnTeamPointUpdate.Broadcast(GetTeamPointsB(), Team);
}

void ACaptureOfFlagGameState::IncrementTime()
{
	Super::IncrementTime();

	if(GetCurrentMatchTime() <= 0)
	{
		UpdateTeamPoints(SecurityTeam, 1, EReasonForEndOfRound::FlagBeSaved);
	}
}

void ACaptureOfFlagGameState::PlayerDead(AController* InstigatorController, AController* LoserController, AActor* DamageCauser)
{
	Super::PlayerDead(InstigatorController, LoserController, DamageCauser);

	ETeam const LoserTeam = IGetPlayerTeamInterface::Execute_FindPlayerTeam(LoserController->PlayerState);
	for(auto& ByArray : PlayerArray)
	{
		if(IGetPlayerTeamInterface::Execute_FindPlayerTeam(ByArray) != LoserTeam) continue;
		if(ByArray->GetPawn()->Controller->GetCharacter() != nullptr) return;
	}
	UpdateTeamPoints(LoserTeam == ETeam::TeamA ? ETeam::TeamB : ETeam::TeamA, 1, EReasonForEndOfRound::AllEnemyDeath);
}
