#include "FlagForCapture.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "SpaceWar/SpaceWarCharacter.h"
#include "SpaceWar/Components/HealthComponent.h"
#include "SpaceWar/GameModes/Match/LastGameMode/CaptureFlagGameMode.h"
#include "SpaceWar/GameStates/Match/CaptureOfFlagGameState.h"
#include "SpaceWar/Interfaces/GetPlayerTeamInterface.h"

AFlagForCapture::AFlagForCapture()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	NetUpdateFrequency = 1.f;

	OwnerCharacter = nullptr;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(RootComponent);
	SphereCollision->SetCollisionProfileName("TeamPointType");
	SphereCollision->SetCollisionResponseToChannels(ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Block);
	SphereCollision->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
	RootComponent = SphereCollision;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMesh->SetupAttachment(SphereCollision);
	StaticMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	StaticMesh->SetCollisionProfileName("NoCollision", false);
}

void AFlagForCapture::BeginPlay()
{
	Super::BeginPlay();

	if(GetLocalRole() == ROLE_Authority)
	{
		SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AFlagForCapture::OnPointCaptureCollisionBeginOverlap);
		auto const GS = Cast<ACaptureOfFlagGameState>(UGameplayStatics::GetGameState(GetWorld()));
		if(GS)
			GS->OnRoundEnded.AddDynamic(this, &AFlagForCapture::RoundEnded);
	}
}

void AFlagForCapture::RoundEnded(const FString& Reason, ETeam WinnerTeam, EReasonForEndOfRound ReasonEndOfRound)
{
	Destroy();
}

void AFlagForCapture::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFlagForCapture, OwnerCharacter);
}


bool AFlagForCapture::InteractionObject_Implementation(ASpaceWarCharacter* Player)
{
	if(OwnerCharacter) return false;

	OwnerCharacter = Player;
	OnRep_CharOwner();
	Player->GetHealthComponent()->OnOwnerDead.AddDynamic(this, &AFlagForCapture::OwnerPlayerDeath);
	ForceNetUpdate();
	return true;
}

void AFlagForCapture::OwnerPlayerDeath()
{
	OwnerCharacter->GetHealthComponent()->OnOwnerDead.RemoveDynamic(this, &AFlagForCapture::OwnerPlayerDeath);
	OwnerCharacter = nullptr;
	OnRep_CharOwner();
	ForceNetUpdate();
}

void AFlagForCapture::OnRep_CharOwner()
{
	if(OwnerCharacter)
	{
		AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "SKT_FlagPoint");
		SetHidden(true);
	}
	else
	{     
		SetHidden(false);
		DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	}
}



void AFlagForCapture::OnPointCaptureCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	//If this Delivery point of flag
	if(!OtherActor->GetClass()->ImplementsInterface(UGetPlayerTeamInterface::StaticClass()) || !OwnerCharacter) return;
	auto const TempTeam = IGetPlayerTeamInterface::Execute_FindPlayerTeam(OwnerCharacter->Controller->PlayerState);
     
	if(IGetPlayerTeamInterface::Execute_FindPlayerTeam(OtherActor) == TempTeam)
	{
		auto const GM = Cast<ACaptureFlagGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if(!GM) return;
     
		GM->UpdateTeamPoints(1, TempTeam, EReasonForEndOfRound::FlagBeCapture);
	}
}