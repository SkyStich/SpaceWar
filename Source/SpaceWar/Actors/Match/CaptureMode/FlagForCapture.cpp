#include "FlagForCapture.h"
#include "Net/UnrealNetwork.h"
#include "SpaceWar/SpaceWarCharacter.h"
#include "SpaceWar/Components/HealthComponent.h"
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
	}
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
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("OVERLAP!!!!!!!!!!"));
}



