// Fill out your copyright notice in the Description page of Project Settings.


#include "OnlineMatchGameModeBase.h"
#include "SpaceWar/Interfaces/UpdateSpecialPointsInterface.h"
#include "SpaceWar/PlayerStates/Match/Base/OnlinePlayerStateBase.h"

AOnlineMatchGameModeBase::AOnlineMatchGameModeBase()
{
}

void AOnlineMatchGameModeBase::UpdateTeamPoints(const int32 Value, ETeam Team)
{
	if(GetGameState<AOnlinetMatchGameStateBase>()->UpdateTeamPoints(Team, Value) >= PointForWin)
	{
		FString const ReasonMessage = FString::Printf(TEXT("Max amount points. %d - winner"), Team); 
		MatchEnded(ReasonMessage, Team);
	}
}

void AOnlineMatchGameModeBase::LaunchGameTimer()
{
	FTimerDelegate TimerDel;
	TimerDel.BindUObject(this, &AOnlineMatchGameModeBase::TickTime, GetGameState<AOnlinetMatchGameStateBase>());
	GetWorld()->GetTimerManager().SetTimer(TimeMatchHandle, TimerDel, 1.f, true);
}

void AOnlineMatchGameModeBase::CharDead(AController* InstigatorController, AController* LoserController, AActor* DamageCauser)
{
	Super::CharDead(InstigatorController, LoserController, DamageCauser);

	UpdatePlayerStatistics(InstigatorController, LoserController);
}

void AOnlineMatchGameModeBase::TickTime(AOnlinetMatchGameStateBase* MatchGameState)
{
	MatchGameState->IncrementTime();
}

void AOnlineMatchGameModeBase::UpdatePlayerStatistics(AController* InstigatorController, AController* LoserController)
{
	if(!LoserController->GetClass()->ImplementsInterface(UUpdateSpecialPointsInterface::StaticClass())) return;
	
	auto const OnlineLoserState = Cast<AOnlinePlayerStateBase>(LoserController->PlayerState);
	if(!InstigatorController || InstigatorController == LoserController)
	{
		OnlineLoserState->DecrementNumberOfMurders();
		OnlineLoserState->IncrementNumberOfDeaths();
		IUpdateSpecialPointsInterface::Execute_DecreaseSpecialPoint(LoserController, 400);
		return;
	}
	auto const OnlineInstigatorState = Cast<AOnlinePlayerStateBase>(InstigatorController->PlayerState);

	OnlineLoserState->IncrementNumberOfDeaths();
	OnlineInstigatorState->IncrementNumberOfMurders();
	IUpdateSpecialPointsInterface::Execute_DecreaseSpecialPoint(LoserController, 200);
	IUpdateSpecialPointsInterface::Execute_IncreaseSpecialPoint(InstigatorController, 150);
}
