#include "CaptureHoldController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

ACaptureHoldController::ACaptureHoldController()
{
	bCanSpawn = true;
}

void ACaptureHoldController::Server_SpawnPlayer_Implementation()
{
	if(!bCanSpawn) return;
	
	TArray<AActor*> Test;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Test);
	SpawnPlayer(Test[0]->GetActorLocation());
}

void ACaptureHoldController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("SpawnPlayer", IE_Released, this, &ACaptureHoldController::Server_SpawnPlayer);
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
