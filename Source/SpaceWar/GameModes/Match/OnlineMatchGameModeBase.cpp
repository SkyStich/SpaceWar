// Fill out your copyright notice in the Description page of Project Settings.


#include "OnlineMatchGameModeBase.h"
#include "SpaceWar/PlayerStates/Match/Base/OnlinePlayerStateBase.h"

void AOnlineMatchGameModeBase::UpdateTeamPoints(const int32 Value, ETeam Team)
{
	if(GetGameState<AOnlinetMatchGameStateBase>()->UpdateTeamPoints(Team, Value) >= 5)
	{
		FString const ReasonMessage = FString::Printf(TEXT("Max amount points. %d - winner"), Team); 
		MatchEnded(ReasonMessage);
	}
}

void AOnlineMatchGameModeBase::CharDead(AController* InstigatorController, AController* LoserController, AActor* DamageCauser)
{
	Super::CharDead(InstigatorController, LoserController, DamageCauser);

	UpdatePlayerStatistics(InstigatorController, LoserController);
}

void AOnlineMatchGameModeBase::UpdatePlayerStatistics(AController* InstigatorController, AController* LoserController)
{
	auto const OnlineLoserState = Cast<AOnlinePlayerStateBase>(LoserController->PlayerState);
	if(!InstigatorController || InstigatorController == LoserController)
	{
		OnlineLoserState->DecrementNumberOfMurders();
		OnlineLoserState->IncrementNumberOfDeaths();
		return;
	}
	auto const OnlineInstigatorState = Cast<AOnlinePlayerStateBase>(InstigatorController->PlayerState);

	OnlineLoserState->IncrementNumberOfDeaths();
	OnlineInstigatorState->IncrementNumberOfMurders();
}
