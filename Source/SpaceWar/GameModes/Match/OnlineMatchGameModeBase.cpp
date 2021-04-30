// Fill out your copyright notice in the Description page of Project Settings.


#include "OnlineMatchGameModeBase.h"
#include "SpaceWar/PlayerStates/Match/Base/OnlinePlayerStateBase.h"

void AOnlineMatchGameModeBase::UpdateTamPoints(const int32 Value, ETeam Team)
{
	GetGameState<AOnlinetMatchGameStateBase>()->UpdateTeamPoints(Team, Value);
}

void AOnlineMatchGameModeBase::CharDead(AController* InstigatorController, AController* LoserController, AActor* DamageCauser)
{
	Super::CharDead(InstigatorController, LoserController, DamageCauser);

	/**Test*/
	auto const PS = InstigatorController->GetPlayerState<AOnlinePlayerStateBase>();
	UpdateTamPoints(1, PS->GetPlayerTeam());
}
