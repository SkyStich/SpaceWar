#include "CaptureHoldController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/HUD.h"
#include "EngineUtils.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SpaceWar/GameModes/Match/OnlineMatchGameModeBase.h"
#include "SpaceWar/GameStates/Match/CaptureHoldGamestate.h"
#include "SpaceWar/Interfaces/ErrorMessageInterface.h"
#include "SpaceWar/PlayerStart/PointCapturePlayerStart.h"

ACaptureHoldController::ACaptureHoldController()
{
	bCanSpawn = true;
}

void ACaptureHoldController::BeginPlay()
{
	Super::BeginPlay();

	Cast<ACaptureHoldGamestate>(UGameplayStatics::GetGameState(GetWorld()))->OnPreMatchEnd.AddDynamic(this, &ACaptureHoldController::MatchEnded);
}

void ACaptureHoldController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("SpawnPlayer", IE_Released, this, &ACaptureHoldController::SpawnPlayerPressed);
}

void ACaptureHoldController::SpawnPlayerPressed()
{
	if(GetCharacter()) return;
	
	OnPreparationSpawnPlayer.Broadcast();
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

void ACaptureHoldController::LaunchRespawnTimer(float const Time)
{
	FTimerDelegate TimerDel;
	TimerDel.BindLambda([&]() -> void
    {
        bCanSpawn = true;
    });
	GetWorld()->GetTimerManager().SetTimer(RespawnTimer, TimerDel, Time, false);
}

bool ACaptureHoldController::SpawnPlayer(const FVector& Location)
{
	GetWorld()->GetTimerManager().ClearTimer(RespawnTimer);
	if(!Super::SpawnPlayer(Location)) return false;

	bCanSpawn = false;
	return true;
}

void ACaptureHoldController::SpawnPlayerByPoint(EPointNumber Point)
{
	if(!bCanSpawn) return;
	
	TArray<APointCapturePlayerStart*>PointArray;
	for(TActorIterator<APointCapturePlayerStart> It(GetWorld(), APointCapturePlayerStart::StaticClass()); It; ++It)
	{
		auto Temp = *It;
		if(Temp->GetPointNumber() == Point && IGetPlayerTeamInterface::Execute_FindPlayerTeam(PlayerState) == Temp->GetSpawnTeam())
		{
			PointArray.Add(Temp);
		}
	}
	Server_SpawnPlayerByPoint(PointArray);
}

void ACaptureHoldController::Server_SpawnPlayerByPoint_Implementation(const TArray<APointCapturePlayerStart*>&PointArray)
{
	auto const GameState = Cast<AOnlinetMatchGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	if(!bCanSpawn) return;
	
	for(auto& ByArray : PointArray)
	{
		if(IGetPlayerTeamInterface::Execute_FindPlayerTeam(PlayerState) != ByArray->GetSpawnTeam()) return;

		if(ByArray->CheckOnFreePoints())
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
}

