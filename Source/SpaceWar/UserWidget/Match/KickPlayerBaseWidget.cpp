// Fill out your copyright notice in the Description page of Project Settings.


#include "KickPlayerBaseWidget.h"
#include "SpaceWar/Interfaces/PlayerControllerInterface.h"
#include "Kismet/GameplayStatics.h"


void UKickPlayerBaseWidget::KickPlayer(APlayerController* Player)
{
    if(GetOwningPlayer() && GetOwningPlayer()->GetClass()->ImplementsInterface(UPlayerControllerInterface::StaticClass()))
        IPlayerControllerInterface::Execute_StartVotingKickPlayer(GetOwningPlayer(), Player);
}
