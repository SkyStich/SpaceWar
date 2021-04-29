// Fill out your copyright notice in the Description page of Project Settings.


#include "OnlinePlayerStateBase.h"
#include "Net/UnrealNetwork.h"

void AOnlinePlayerStateBase::SetTeam(const ETeam NewTeam)
{
	PlayerTeam = NewTeam;
}

void AOnlinePlayerStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AOnlinePlayerStateBase, PlayerTeam);
}
