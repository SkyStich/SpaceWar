// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChatMatchComponent.generated.h"

class AMatchPlayerControllerBase;

/** only server component */

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEWAR_API UChatMatchComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UChatMatchComponent();

	void SendMessageAllPlayer(const FString& Message, AMatchPlayerControllerBase* Owner);
	void SendMessageFromAnAlly(const FString& Message, AMatchPlayerControllerBase* Owner);
};
