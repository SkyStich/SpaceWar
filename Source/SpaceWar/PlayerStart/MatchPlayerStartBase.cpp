#include "MatchPlayerStartBase.h"
#include "Components/CapsuleComponent.h"
#include "SpaceWar/SpaceWarCharacter.h"
#include "SpaceWar/Components/HealthComponent.h"

void AMatchPlayerStartBase::BeginPlay()
{
	Super::BeginPlay();
}

bool AMatchPlayerStartBase::CheckOnFreePoints() const
{
	TArray<AActor*> OverlapActors;
	GetCapsuleComponent()->GetOverlappingActors(OverlapActors, ASpaceWarCharacter::StaticClass());

	for(auto& ByArray : OverlapActors)
	{
		auto const TempCharacter = Cast<ASpaceWarCharacter>(ByArray);
		if(TempCharacter && !TempCharacter->GetHealthComponent()->IsOwnerDeath()) return false;
	}
	return true;
}