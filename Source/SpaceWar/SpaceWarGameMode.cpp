// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpaceWarGameMode.h"
#include "SpaceWarCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASpaceWarGameMode::ASpaceWarGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
