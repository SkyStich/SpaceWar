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
	DOREPLIFETIME(AOnlinePlayerStateBase, NumberOfDeaths);
	DOREPLIFETIME(AOnlinePlayerStateBase, NumberOfMurders);
}

void AOnlinePlayerStateBase::IncrementNumberOfDeaths()
{
	NumberOfDeaths++;
	OnRep_NumberOfDeaths();
}

//void AOnlinePlayerStateBase::DecrementNumberOfDeaths()
//{
	//NumberOfDeaths--;
	//OnRep_NumberOfDeaths();
//}

void AOnlinePlayerStateBase::DecrementNumberOfMurders()
{
	NumberOfMurders--;
	OnRep_NumberOfMurders();
}

void AOnlinePlayerStateBase::IncrementNumberOfMurders()
{
	NumberOfMurders++;
	OnRep_NumberOfMurders();
}

void AOnlinePlayerStateBase::OnRep_NumberOfDeaths()
{
	OnNumberOfDeathsChanged.Broadcast();
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("OnRep_NumberOfDeaths"));
}

void AOnlinePlayerStateBase::OnRep_NumberOfMurders()
{
	OnNumberOfMurdersChanged.Broadcast();
}


