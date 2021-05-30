#include "SpecialTurelController.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"
#include "Perception/AIPerceptionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISense_Sight.h"

ASpecialTurelController::ASpecialTurelController()
{
	Perception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
}

void ASpecialTurelController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	TurelPawn = Cast<ABaseTurelPawn>(InPawn);

	if(GetLocalRole() != ROLE_Authority) return;
	TurelPawn->ObjectHealthComponent->OnHealthEnded.AddDynamic(this, &ASpecialTurelController::ObjectDead);
	Perception->OnTargetPerceptionUpdated.AddDynamic(this, &ASpecialTurelController::OnPerceptionUpdate);
}

void ASpecialTurelController::ObjectDead()
{
	Perception->OnTargetPerceptionUpdated.RemoveAll(this);
	TurelPawn->TargetActor = nullptr;
	StopObjectUse(false);
}

void ASpecialTurelController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ASpecialTurelController::OnPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus)
{
	if(Stimulus.WasSuccessfullySensed())
	{
		if(TurelPawn->bObjectUsed) return;

		auto const PS = Actor->GetInstigatorController()->GetPlayerState<AOnlinePlayerStateBase>();
		if(!PS || PS->GetPlayerTeam() == TurelPawn->Team) return;

		GetWorld()->GetTimerManager().ClearTimer(PerceptionRefreshHandle);
		TurelPawn->TargetActor = Actor;
		LaunchObjectUsed();
	}
	else
	{
		if(TurelPawn->TargetActor == Actor)
		{
			FTimerDelegate TimerDel;
			TimerDel.BindLambda([&]() -> void 
			{
				GetWorld()->GetTimerManager().ClearTimer(PerceptionRefreshHandle);
				TurelPawn->TargetActor = nullptr;
			});

			GetWorld()->GetTimerManager().SetTimer(PerceptionRefreshHandle, TimerDel, 1.f, false);
			StopObjectUse(false);
		}
	}
}

AActor* ASpecialTurelController::GetFirstPerceptionByTeam()
{
	TArray<AActor*> Actors;
	Perception->GetKnownPerceivedActors(UAISense_Sight::StaticClass(), Actors);
	for(auto& ByArray : Actors)
	{
		auto const PS = ByArray->GetInstigatorController()->GetPlayerState<AOnlinePlayerStateBase>();
		if(PS && PS->GetPlayerTeam() != TurelPawn->Team)
		{
			return ByArray;
		}
	}
	return nullptr;
}

void ASpecialTurelController::DropTraceForTarget()
{
	if(!TurelPawn->TargetActor) return;

	FCollisionQueryParams Query;
	Query.AddIgnoredActor(this);
	FHitResult OutHit;
	if(GetWorld()->LineTraceSingleByChannel(OutHit, TurelPawn->GetActorLocation(), TurelPawn->TargetActor->GetActorLocation(), ECC_Visibility, Query))
	{
		FVector const HitFromDirection = (OutHit.TraceEnd - OutHit.TraceStart).GetSafeNormal();
		UGameplayStatics::ApplyPointDamage(TurelPawn->TargetActor, TurelPawn->BaseDamage, HitFromDirection, OutHit, TurelPawn->OwnerController, this, UDamageType::StaticClass());
	}
	Test(OutHit);
}

void ASpecialTurelController::Test_Implementation(const FHitResult& Hit)
{
	DrawDebugLine(GetWorld(), Hit.TraceStart, Hit.TraceEnd, FColor::Green, false, 1.f);
	DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 6.f, 12, FColor::Yellow, false, 1.f);
}

void ASpecialTurelController::UseObject()
{
	FTimerHandle RateHandle;
	GetWorld()->GetTimerManager().SetTimer(RateHandle, this, &ASpecialTurelController::StopRateDelay, TurelPawn->DelayBeforeUse, false);
	DropTraceForTarget();
}

void ASpecialTurelController::LaunchObjectUsed()
{
	if(!TurelPawn->TargetActor) return;
	
	FTimerDelegate TimerDel;
	TimerDel.BindUObject(this, &ASpecialTurelController::StopObjectUse, true);
	GetWorld()->GetTimerManager().SetTimer(TargetCoolDawnHandle, TimerDel, 4.f, false);
	TurelPawn->bObjectUsed = true;
	TurelPawn->OnRep_ObjectUsed();
	UseObject();
}

void ASpecialTurelController::StopObjectUse(bool bRefresh)
{
	GetWorld()->GetTimerManager().ClearTimer(TargetCoolDawnHandle);
	TurelPawn->bObjectUsed = false;
	TurelPawn->OnRep_ObjectUsed();
	if(bRefresh)
	{
		FTimerHandle Refresh;
		GetWorld()->GetTimerManager().SetTimer(Refresh, this, &ASpecialTurelController::LaunchObjectUsed, 1.f, false);
	}
}

void ASpecialTurelController::StopRateDelay()
{
	if(GetWorld()->GetTimerManager().IsTimerActive(TargetCoolDawnHandle))
	{
		UseObject();
	}
}