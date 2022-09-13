#include "CaptureHoldController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/HUD.h"
#include "EngineUtils.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SpaceWar/GameModes/Match/OnlineMatchGameModeBase.h"
#include "SpaceWar/GameStates/Match/CaptureHoldGamestate.h"
#include "SpaceWar/HUD/Match/CaptureAndHoldHUD.h"
#include "SpaceWar/Interfaces/ErrorMessageInterface.h"
#include "SpaceWar/PlayerStart/PointCapturePlayerStart.h"

ACaptureHoldController::ACaptureHoldController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCanSpawn = false;
	
	RespawnTime = 15.f;
}

void ACaptureHoldController::BeginPlay()
{
	Super::BeginPlay();

	auto const GS = Cast<ACaptureHoldGamestate>(UGameplayStatics::GetGameState(GetWorld()));

	GS->OnPreMatchEnd.AddDynamic(this, &ACaptureHoldController::MatchEnded);
	
	if(GetLocalRole() == ROLE_Authority)
	{
		if(GS->GameInProgress())
		{
			bCanSpawn = true;
		}
		else
		{
			GS->OnPreparationStartGameFinish.AddDynamic(this, &ACaptureHoldController::OnPreparationStartGameFinishEvent);
		}
	}
}

void ACaptureHoldController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ACaptureHoldController, bCanSpawn, COND_OwnerOnly);
}

void ACaptureHoldController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("SpawnPlayer", IE_Released, this, &ACaptureHoldController::SpawnPlayerPressed);
}

void ACaptureHoldController::SpawnPlayerPressed()
{
	if(!GetCharacter())
	{
		OnPreparationSpawnPlayer.Broadcast();
	}
}

void ACaptureHoldController::MatchEnded(const FString& Reason, ETeam WinnerTeam)
{
	if(GetCharacter())
	{
		GetCharacter()->GetCharacterMovement()->StopMovementImmediately();
		GetCharacter()->GetCharacterMovement()->DisableMovement();
		
		if(GetLocalRole() != ROLE_Authority)
			GetCharacter()->InputComponent->ClearActionBindings();
	}
}

void ACaptureHoldController::LaunchRespawnTimer()
{
	FTimerDelegate TimerDel;
	TimerDel.BindLambda([&]() -> void { bCanSpawn = true; });
	GetWorld()->GetTimerManager().SetTimer(RespawnTimer, TimerDel, RespawnTime, false);
}

bool ACaptureHoldController::SpawnPlayer(const FVector& Location)
{
	GetWorld()->GetTimerManager().ClearTimer(RespawnTimer);
	if(!Super::SpawnPlayer(Location)) return false;
	
	bCanSpawn = false;
	return true;
}

bool ACaptureHoldController::SpawnPlayerByPoint(EPointNumber Point)
{
	if(!bCanSpawn) return false;
	
	TArray<APointCapturePlayerStart*>PointArray;
	for(TActorIterator<APointCapturePlayerStart> It(GetWorld(), APointCapturePlayerStart::StaticClass()); It; ++It)
	{
		auto Temp = *It;
		if(Temp->GetPointNumber() == Point && Temp->CheckOnFreePoints() && IGetPlayerTeamInterface::Execute_FindPlayerTeam(PlayerState) == Temp->GetSpawnTeam())
		{
			PointArray.Add(Temp);
		}
	}
	Server_SpawnPlayerByPoint(PointArray);
	return true;
}

void ACaptureHoldController::Server_SpawnPlayerByPoint_Implementation(const TArray<APointCapturePlayerStart*>&PointArray)
{
	if(!bCanSpawn)
	{
		Client_ErrorMessage(TEXT("You can not spawn"));
		return;
	}
	
	for(auto& ByArray : PointArray)
	{
		if(ByArray->CheckOnFreePoints() && IGetPlayerTeamInterface::Execute_FindPlayerTeam(PlayerState) == ByArray->GetSpawnTeam())
		{
			SpawnPlayer(ByArray->GetActorLocation());
			return;
		}
	}
	Client_ErrorMessage(TEXT("No free drop - of points"));
}

void ACaptureHoldController::Client_ErrorMessage_Implementation(const FString& Message)
{
	IErrorMessageInterface::Execute_ClientErrorMessage(MyHUD, Message);
	GetHUD<ACaptureAndHoldHUD>()->SpawnPlayerCompleteWithFail();
}

void ACaptureHoldController::OnPreparationStartGameFinishEvent(bool bResult)
{
	bCanSpawn = bResult;
}