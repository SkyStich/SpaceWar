#include "BaseMineProjectile.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerState.h"
#include "SpaceWar/Interfaces/GetPlayerTeamInterface.h"

ABaseMineProjectile::ABaseMineProjectile()
{
	DetonationCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("DetonationBox"));
	DetonationCollision->SetupAttachment(RootComponent);
	DetonationCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	DetonationCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ABaseMineProjectile::BeginPlay()
{
	if(GetLocalRole() == ROLE_Authority)
	{
		DetonationCollision->OnComponentBeginOverlap.AddDynamic(this, &ABaseMineProjectile::OnPointCaptureCollisionBeginOverlap);
	}
}

void ABaseMineProjectile::OnPointCaptureCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	auto const Character = Cast<ACharacter>(OtherActor);
	if(!OtherActor) return;

	ETeam const OwnerTeam = IGetPlayerTeamInterface::Execute_FindPlayerTeam(OwnerPlayerController->PlayerState);
	if(OwnerTeam == IGetPlayerTeamInterface::Execute_FindPlayerTeam(Character->Controller->PlayerState)) return;

	Detonation(ThrowData->BaseDamage, ThrowData->ExpRange, OwnerPlayerController);
}

void ABaseMineProjectile::Init(AController* OwnerController, FBaseThrowData* SpecialWeaponData)
{
	OwnerPlayerController = OwnerController;
	ThrowData = SpecialWeaponData;
}
