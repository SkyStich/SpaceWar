// Fill out your copyright notice in the Description page of Project Settings.


#include "MatchPlayerControllerBase.h"
#include "SpaceWar/HUD/Match/BaseMatchHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "UObject/ConstructorHelpers.h"
#include "SpaceWar/SpaceWarCharacter.h"
#include "SpaceWar/GameModes/Match/Base/MatchGameModeBase.h"

AMatchPlayerControllerBase::AMatchPlayerControllerBase()
{
	bReplicates = true;
	NetUpdateFrequency = 5.f;
	bCanSpawn = true;

	static ConstructorHelpers::FClassFinder<ASpaceWarCharacter> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if(PlayerPawnBPClass.Class != nullptr)
	{
		PlayerClass = PlayerPawnBPClass.Class;
	}
}

void AMatchPlayerControllerBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMatchPlayerControllerBase, PlayerClass);
	DOREPLIFETIME_CONDITION(AMatchPlayerControllerBase, bCanSpawn, COND_OwnerOnly);
}

void AMatchPlayerControllerBase::SetPlayerClass(TSubclassOf<ASpaceWarCharacter> NewPlayerClass)
{
	if(GetLocalRole() == ROLE_Authority)
	{
		PlayerClass = NewPlayerClass;
	}
}

void AMatchPlayerControllerBase::SpawnPlayer()
{
	if(!bCanSpawn) return;

	ASpaceWarCharacter* CharacterForSpawn;
	auto const GM = Cast<AMatchGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if(GM)
	{
		GM->SpawnCharacter(this, CharacterForSpawn);
		bCanSpawn = false;
	}	
}

void AMatchPlayerControllerBase::LaunchRespawnTimer(float const Time)
{
	FTimerDelegate TimerDel;
	TimerDel.BindLambda([&]() -> void
	{
		bCanSpawn = true;
	});
	GetWorld()->GetTimerManager().SetTimer(RespawnTimer, TimerDel, Time, false);
}

void AMatchPlayerControllerBase::Server_SpawnPlayerPressed_Implementation()
{
	GetWorld()->GetTimerManager().ClearTimer(RespawnTimer);
	SpawnPlayer();
}

void AMatchPlayerControllerBase::SetupInputComponent()
{
	if(!InputComponent)
	{
		InputComponent = NewObject<UInputComponent>(this);
		InputComponent->RegisterComponent();
	}

	InputComponent->BindAction("SpawnPlayer", IE_Released, this, &AMatchPlayerControllerBase::Server_SpawnPlayerPressed);

	InputComponent->BindAction("TabMenu", IE_Pressed, this, &AMatchPlayerControllerBase::PressTabMenu);
	InputComponent->BindAction("TabMenu", IE_Released, this, &AMatchPlayerControllerBase::ReleasedTabMenu);
}

void AMatchPlayerControllerBase::OnRep_Pawn()
{
	Super::OnRep_Pawn();
	
	if(GetLocalRole() != ROLE_Authority)
	{
		OnNewPawn.Broadcast(GetPawn());
	}
}

void AMatchPlayerControllerBase::PressTabMenu()
{
	GetHUD<ABaseMatchHUD>()->ShowTabMenu();
}

void AMatchPlayerControllerBase::ReleasedTabMenu()
{
	GetHUD<ABaseMatchHUD>()->HiddenTabMenu();
}


