// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUBGameModeBase.h"
#include "GameFramework/GameState.h"
#include "GameFramework/PlayerState.h"

AMainHUBGameModeBase::AMainHUBGameModeBase()
{
	HubDataBaseComponent = CreateDefaultSubobject<UHubServerDataBaseComponent>(TEXT("HubServerDataBaseComponent"));
}

void AMainHUBGameModeBase::BeginPlay()
{
	Super::BeginPlay(); 
	 
	HubDataBaseComponent->OnServerDeactivate.AddDynamic(this, &AMainHUBGameModeBase::ServerDeactivate);
	HubDataBaseComponent->OnForcedServerShutdown.AddDynamic(this, &AMainHUBGameModeBase::ManuallyShuttingDownServer);
}

void AMainHUBGameModeBase::ServerDeactivate()
{
	UE_LOG(LogTemp, Warning, TEXT("OnServerDeactivateEvent"));
	for(const auto& ByArray : GameState->PlayerArray)
	{
		if(!ByArray) continue;
		auto const PC = Cast<APlayerController>(ByArray->GetOwner());
		if(!PC) continue;
		
		PC->ClientWasKicked(FText::FromString("Server Deactivate"));
	}
}

void AMainHUBGameModeBase::ManuallyShuttingDownServer()
{
	UE_LOG(LogTemp, Warning, TEXT("OnServerShutdown"));
	for(const auto& ByArray : GameState->PlayerArray)
	{
		if(!ByArray) continue;
		auto const PC = Cast<APlayerController>(ByArray->GetOwner());
		if(!PC) continue;
		
		PC->ClientWasKicked(FText::FromString("The server a shutdown"));
	}
	HubDataBaseComponent->ShutDownServer("The server a shutdown");
}
