// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmunitionSpecialBase.h"
#include "SpaceWar/Interfaces/GetPlayerTeamInterface.h"
#include "SpaceWar/SpaceWarCharacter.h"
#include "GameFramework/PlayerState.h"

bool AAmmunitionSpecialBase::InteractionObject_Implementation(ASpaceWarCharacter* Player)
{
	if(!Super::InteractionObject_Implementation(Player)) return false;
	
	auto const PS = Player->Controller->PlayerState;
	if(IGetPlayerTeamInterface::Execute_FindPlayerTeam(PS) != Team) return false;

	OwnerController->UseAmmunitionState();
	return true;
}