// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamPoints.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "SpaceWar/GameModes/Match/OnlineMatchGameModeBase.h"
#include "SpaceWar/GameStates/Match/OnlinetMatchGameStateBase.h"


// Sets default values
ATeamPoints::ATeamPoints()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	OwnerTeam = ETeam::NoneTeam;
	bIsCapture = false;
	CurrentAmountOwnerInPoint = 0;
	CurrentAmountEnemyAtPoint = 0;

	bReplicates = true;
	NetUpdateFrequency = 10.f;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	PointCaptureCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Point collision"));
	PointCaptureCollision->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATeamPoints::BeginPlay()
{
	Super::BeginPlay();

	if(GetLocalRole() == ROLE_Authority)
	{
		PointCaptureCollision->OnComponentBeginOverlap.AddDynamic(this, &ATeamPoints::OnPointCaptureCollisionBeginOverlap);
		PointCaptureCollision->OnComponentEndOverlap.AddDynamic(this, &ATeamPoints::OnPointCaptureCollisionEndOverlap);
	}
}

void ATeamPoints::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATeamPoints, bIsCapture);
	DOREPLIFETIME(ATeamPoints, OwnerTeam);
	DOREPLIFETIME(ATeamPoints, CurrentValueCapture);
}


void ATeamPoints::OnPointCaptureCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor);
	if(!OtherCharacter) return;

	auto const OtherState = Cast<AOnlinePlayerStateBase>(OtherCharacter->Controller->PlayerState);
	
	if(OtherState->GetPlayerTeam() == OwnerTeam)
	{
		CurrentAmountOwnerInPoint++;

		if(CurrentAmountOwnerInPoint > 1 && CurrentValueCapture >= 100) return;
		
		if(CurrentAmountEnemyAtPoint <= 0)
		{
			RefreshLaunchCapture();
		}
		else
		{
			GetWorld()->GetTimerManager().ClearTimer(CaptureHandle);
		}
		return;
	}
	
	/** Other player */
	CurrentAmountEnemyAtPoint++;
	if(CurrentAmountOwnerInPoint > 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(CaptureHandle);
		return;
	}
	
	if(!GetWorld()->GetTimerManager().IsTimerActive(CaptureHandle))
	{
		LaunchPointCapture(OtherState->GetPlayerTeam());
	}
}

void ATeamPoints::OnPointCaptureCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor);
	if(!OtherCharacter) return;

	auto const OtherState = Cast<AOnlinePlayerStateBase>(OtherCharacter->Controller->PlayerState);

	if(OtherState->GetPlayerTeam() == OwnerTeam)
	{
		CurrentAmountOwnerInPoint--;
		if(CurrentAmountOwnerInPoint <= 0 && CurrentValueCapture <= 100)
		{
			if(CurrentAmountEnemyAtPoint <= 0)
			{
				GetWorld()->GetTimerManager().ClearTimer(CaptureHandle);
			}
			else
			{
				LaunchPointCapture(OwnerTeam == ETeam::TeamA ? ETeam::TeamB : ETeam::TeamA);
			}
		}
		return;
	}
	
	CurrentAmountEnemyAtPoint--;
	if(CurrentAmountEnemyAtPoint <= 0 && CurrentAmountOwnerInPoint > 0)
	{
		RefreshLaunchCapture();
	}
}

void ATeamPoints::RefreshLaunchCapture()
{
	GetWorld()->GetTimerManager().SetTimer(CaptureHandle, this, &ATeamPoints::IncreaseCurrentValueCapture, 0.1, true);
}

void ATeamPoints::OnRep_TeamPoints()
{
	
}

void ATeamPoints::LaunchPointCapture(ETeam CaptureTeam)
{
	if(CurrentValueCapture > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(CaptureHandle, this, &ATeamPoints::DecreaseCurrentValueCapture, 0.1, true);
	}
	else
	{
		CurrentAmountOwnerInPoint = CurrentAmountEnemyAtPoint;
		CurrentAmountEnemyAtPoint = 0;
		OwnerTeam = CaptureTeam;
		OnRep_TeamPoints();
		GetWorld()->GetTimerManager().SetTimer(CaptureHandle, this, &ATeamPoints::IncreaseCurrentValueCapture, 0.1, true);
	}
}

void ATeamPoints::StopPointCapture()
{
	GetWorld()->GetTimerManager().ClearTimer(CaptureHandle);
}

void ATeamPoints::IncreaseCurrentValueCapture()
{
	CurrentValueCapture += 1 * FMath::Min(CurrentAmountOwnerInPoint, 5);
	if(CurrentValueCapture >= 100)
	{
		GetWorld()->GetTimerManager().ClearTimer(CaptureHandle);
		bIsCapture = true;
		StartAddPoint();
	}
}

void ATeamPoints::DecreaseCurrentValueCapture()
{
	CurrentValueCapture -= 1 * FMath::Min(CurrentAmountEnemyAtPoint, 5);

	if(CurrentValueCapture <= 0)
	{
		bIsCapture = false;
		
		GetWorld()->GetTimerManager().ClearTimer(CaptureHandle);
		GetWorld()->GetTimerManager().ClearTimer(AddPointHandle);
		
		LaunchPointCapture(OwnerTeam == ETeam::TeamA ? ETeam::TeamB : ETeam::TeamA);
	}
}

void ATeamPoints::StartAddPoint()
{
	if(GetWorld()->GetTimerManager().IsTimerActive(AddPointHandle)) return;
	
	auto const GameMode = Cast<AOnlineMatchGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	
	FTimerDelegate TimerDel;
	TimerDel.BindUObject(this, &ATeamPoints::UpdateTeamPoints, GameMode);
	GetWorld()->GetTimerManager().SetTimer(AddPointHandle, TimerDel, 1.f, true);
}

void ATeamPoints::UpdateTeamPoints(AOnlineMatchGameModeBase* OnlineGameMode)
{
	OnlineGameMode->UpdateTeamPoints(1, OwnerTeam);
}