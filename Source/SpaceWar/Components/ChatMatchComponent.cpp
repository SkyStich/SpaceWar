// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatMatchComponent.h"
#include "SpaceWar/Interfaces/MatchChatInterface.h"
#include "GameFramework/PlayerState.h"
#include "SpaceWar/Interfaces/GetPlayerTeamInterface.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameState.h"
#include "SpaceWar/PlayerControllers/Match/Base/MatchPlayerControllerBase.h"

// Sets default values for this component's properties
UChatMatchComponent::UChatMatchComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UChatMatchComponent::SendMessageAllPlayer(const FString& Message, AMatchPlayerControllerBase* Owner)
{
	auto const GS = UGameplayStatics::GetGameState(GetWorld());

	for(const auto& ByArray : GS->PlayerArray)
	{
		if(ByArray->GetOwner()->GetClass()->ImplementsInterface(UMatchChatInterface::StaticClass()))
		{
			IMatchChatInterface::Execute_GetMessage(ByArray->GetOwner(), Message, false);
		}
	}
}

void UChatMatchComponent::SendMessageFromAnAlly(const FString& Message, AMatchPlayerControllerBase* Owner)
{
	if(!Owner) return;
	
	auto const GS = UGameplayStatics::GetGameState(GetWorld());
	ETeam const OwnerTeam = IGetPlayerTeamInterface::Execute_FindPlayerTeam(Owner->PlayerState);

	for(const auto& ByArray : GS->PlayerArray)
	{
		if(IGetPlayerTeamInterface::Execute_FindPlayerTeam(ByArray) == OwnerTeam)
		{
			IMatchChatInterface::Execute_GetMessage(ByArray->GetOwner(), Message, true);
		}
	}
}
