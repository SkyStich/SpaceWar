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

	if(GetLocalRole() == ROLE_Authority)
	{
		RefreshRound();
	}
}

void ACaptureOfFlagGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACaptureOfFlagGameState, SecurityTeam);
	DOREPLIFETIME(ACaptureOfFlagGameState, CurrentPreparationTime);
}

void ACaptureOfFlagGameState::RefreshRound()
{
	CurrentPreparationTime = 5;
	SecurityTeam = (GetTeamPointsA() + GetTeamPointsB()) % 2 == 0 ? ETeam::TeamA : ETeam::TeamB;
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("SecurityTeam: %hhd"), SecurityTeam));
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

void ACaptureOfFlagGameState::MatchFinish_Implementation(const FString& Reason)
{
	Super::MatchFinish_Implementation(Reason);

	GetWorld()->GetTimerManager().ClearTimer(RefreshRoundHandle);
}

void ACaptureOfFlagGameState::NetMulticast_RoundEnded_Implementation()
{
	if(GetLocalRole() == ROLE_Authority)
	{
		GetWorld()->GetTimerManager().ClearTimer(RefreshRoundHandle);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("End round"));
		GetWorld()->GetTimerManager().SetTimer(RefreshRoundHandle, this, &ACaptureOfFlagGameState::RefreshRound, 5.f, false);
	}

	OnRoundEnded.Broadcast();
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

int32 ACaptureOfFlagGameState::UpdateTeamPoints(ETeam Team, int32 Value)
{
	NetMulticast_RoundEnded();
	return Super::UpdateTeamPoints(Team, Value);
}

void ACaptureOfFlagGameState::IncrementTime()
{
	Super::IncrementTime();

	if(GetCurrentMatchTime() <= 0)
	{
		UpdateTeamPoints(SecurityTeam, 1);
	}
}

void ACaptureOfFlagGameState::PlayerDead(AController* InstigatorController, AController* LoserController, AActor* DamageCauser)
{
	Super::PlayerDead(InstigatorController, LoserController, DamageCauser);
	
	ETeam const LoserTeam = IGetPlayerTeamInterface::Execute_FindPlayerTeam(LoserController->PlayerState);
	for(auto& ByArray : PlayerArray)
	{
		if(IGetPlayerTeamInterface::Execute_FindPlayerTeam(ByArray) != IGetPlayerTeamInterface::Execute_FindPlayerTeam(LoserController->PlayerState)) continue;
		if(ByArray->GetPawn()->GetController()->GetCharacter()) return;
	}
	UpdateTeamPoints(LoserTeam == ETeam::TeamA ? ETeam::TeamB : ETeam::TeamA, 1);
}
