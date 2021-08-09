// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpaceWar/Structs/CreateServerCallBack.h"

#include "GameServerDataBaseComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEWAR_API UGameServerDataBaseComponent : public UActorComponent
{
	GENERATED_BODY()

	UFUNCTION()
	void OnResponseCreateServer(bool bResult);

	UFUNCTION()
	void  OnResponseServerAddress(const FString& Address);

public:	
	// Sets default values for this component's properties
	UGameServerDataBaseComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void CallGameServer(const FGameAddressCallBack& CallBack);
	void CreateServerInDataBase(const FString& Address);
	
	/** removes the server from the database when it is turned off */
	void RemoveServerFromDataBase();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	UPROPERTY()
	FGameAddressCallBack GameAddressCallBack;
	
	FString LevelName;
	FString ServerName;
};
