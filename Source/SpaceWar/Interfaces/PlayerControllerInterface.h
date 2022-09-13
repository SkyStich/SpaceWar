#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SpaceWar/Enums/PlayerTeamEnum.h"
#include "PlayerControllerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerControllerInterface : public UInterface
{
	GENERATED_BODY()
};

class SPACEWAR_API IPlayerControllerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, BlueprintAuthorityOnly)
	bool IsPLayerCharacterAlive();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StartVotingKickPlayer(APlayerController* KickPlayer);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void VotingStartedForKickPlayer();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TransferSpawnLocationByPointNumber(EPointNumber NumberPoint);
};
