// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SpaceWar/Enums/PlayerTeamEnum.h"
#include "MatchGameModeBase.generated.h"

class ASpaceWarCharacter;
class AMatchPlayerControllerBase;
class AGameStateMatchGame;
class UGameServerDataBaseComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FPlayerDead, AController*, InstegatedBy, AController*, LoserController, AActor*, DamageCauser);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMatchEnded, const FString&, Reason, ETeam, TeamWinner);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerPostLogin, APlayerController*, PlayerConttroller);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPreMatchEnded, const FString&, Reason, ETeam, TeamWinner);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPreServerShutdown);

UCLASS(Abstract)
class SPACEWAR_API AMatchGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

private:

	UFUNCTION()
	void GameFinish(FString Reason, ETeam WinnerTeam);
	
	UFUNCTION()
	void AsyncSpawnSpectatorComplete(FSoftObjectPath Reference, FTransform SpawnTransform, AController* Controller);

	UFUNCTION()
	void OnForcedServerShutdownEvent();

public:

	AMatchGameModeBase();

	virtual void CharDead(AController* InstigatorController, AController* LoserController, AActor* DamageCauser);
	virtual void SpawnCharacter(AMatchPlayerControllerBase* Controller, const FVector& Location);
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void SpawnSpectator(AController* PossessController, const FVector& Location, const FRotator& Rotation);
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	UFUNCTION()
	UGameServerDataBaseComponent* GetDataBaseComponent() const { return GameDataBaseComponent; }

protected:

	virtual void MatchEnded(const FString& Reason, ETeam WinnerTeam);
	virtual void RespawnPlayer(AController* RespawnController, float const Time) {}
	virtual void RespawnPlayer(AController* RespawnController) {}
	virtual void BeginPlay() override;
	virtual void Logout(AController* Exiting) override;
	
public:

	FPlayerDead OnPlayerDead;

	UPROPERTY()
	FMatchEnded OnMatchEnded;

	UPROPERTY()
	FPreMatchEnded OnPreMatchEnded;

	UPROPERTY()
	FPlayerPostLogin OnPlayerPostLogin;
	
	UPROPERTY()
	FPreServerShutdown OnPreServerShutdown;

protected:

	UPROPERTY(EditAnywhere)
	int32 PointForWin;

	UPROPERTY(VisibleDefaultsOnly)
	UGameServerDataBaseComponent* GameDataBaseComponent;

	FTimerHandle TimeMatchHandle;
};
