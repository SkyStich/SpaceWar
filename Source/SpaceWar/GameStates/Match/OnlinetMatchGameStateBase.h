// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Base/GameStateMatchGame.h"
#include "OnlinetMatchGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API AOnlinetMatchGameStateBase : public AGameStateMatchGame
{
	GENERATED_BODY()

	void SetTeamForPlayer(APlayerController* PC);

protected:

	virtual void NewPlayerLogin(APlayerController* PC) override;

	/** Call if player log out */
	virtual void AutoBalanceTeam();
	
};
