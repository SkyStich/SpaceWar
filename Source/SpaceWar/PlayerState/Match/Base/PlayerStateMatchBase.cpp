// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateMatchBase.h"
#include "SpaceWar/GameModes/Match/Base/MatchGameModeBase.h"

APlayerStateMatchBase::APlayerStateMatchBase()
{
	MaxMatchTime = 10;
	CurrentMatchTime = MaxMatchTime;
}

void APlayerStateMatchBase::BeginPlay()
{
	Super::BeginPlay();

	if(GetLocalRole() == ROLE_Authority)
	{
		Cast<AMatchGameModeBase>(AuthorityGameMode)->OnMatchEnded.AddDynamic(this, &APlayerStateMatchBase::MatchFinish);	
	}
}

void APlayerStateMatchBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void APlayerStateMatchBase::IncrementTime()
{
	CurrentMatchTime--;
}

void APlayerStateMatchBase::MatchFinish_Implementation(const FString& Reason)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, Reason);
}
