// Fill out your copyright notice in the Description page of Project Settings.


#include "MatchPlayerControllerBase.h"
#include "Blueprint/UserWidget.h"
#include "SpaceWar/HUD/Match/BaseMatchHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "UObject/ConstructorHelpers.h"
#include "SpaceWar/Actors/Match/SpecialWeapon/SpecialWeaponObjectBase.h"
#include "SpaceWar/SpaceWarCharacter.h"
#include "SpaceWar/GameModes/Match/OnlineMatchGameModeBase.h"
#include "SpaceWar/Components/GameMode/GameServerDataBaseComponent.h"

AMatchPlayerControllerBase::AMatchPlayerControllerBase()
{
	bReplicates = true;
	NetUpdateFrequency = 2.f;

	DecreaseSpecialPointsValue = 200;
	bAttachToPawn = true;

	SpecialObjectManager = CreateDefaultSubobject<USpecialObjectManagerComponent>(TEXT("SpecialObjectManager"));

	static ConstructorHelpers::FClassFinder<ASpaceWarCharacter> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if(PlayerPawnBPClass.Succeeded())
	{
		PlayerClass = PlayerPawnBPClass.Class;
	}
}

void AMatchPlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();

	if(GetLocalRole() == ROLE_Authority)
	{
		CreateChatComponent();
		auto const GM = Cast<AMatchGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		GM->OnMatchEnded.AddDynamic(this, &AMatchPlayerControllerBase::EndMatch);
	//	GM->GetDataBaseComponent()->OnForcedServerShutdown.AddDynamic(this, &AMatchPlayerControllerBase::ForcedDisconnectFromServer);
	}
}

void AMatchPlayerControllerBase::CreateChatComponent()
{
	/** Create chat component */
	if(GetWorld()->bIsTearingDown)
	{
		UE_LOG(LogActor, Warning, TEXT("AddComponent failed because we are in the process of tearing down the world"));
		return;
	}

	ChatComponent = NewObject<UChatMatchComponent>(this);
	PostCreateBlueprintComponent(ChatComponent);
}

void AMatchPlayerControllerBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMatchPlayerControllerBase, PlayerClass);
	//DOREPLIFETIME_CONDITION(AMatchPlayerControllerBase, bCanSpawn, COND_OwnerOnly);
}

void AMatchPlayerControllerBase::SetPlayerClass(TSubclassOf<ASpaceWarCharacter> NewPlayerClass)
{
	if(GetLocalRole() == ROLE_Authority)
	{
		PlayerClass = NewPlayerClass;
	}
}

bool AMatchPlayerControllerBase::SpawnPlayer(const FVector& Location)
{
	auto const GM = Cast<AOnlineMatchGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	bool const bInProgress = Cast<AOnlinetMatchGameStateBase>(UGameplayStatics::GetGameState(GetWorld()))->GameInProgress();
	if(GM && bInProgress)
	{
		GM->SpawnCharacter(this, Location);
		return true;
	}
	return false;
}

void AMatchPlayerControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AMatchPlayerControllerBase::SetupInputComponent()
{
	if(!InputComponent)
	{
		InputComponent = NewObject<UInputComponent>(this);
		InputComponent->RegisterComponent();
	}

	check(InputComponent);

	InputComponent->BindAction("TabMenu", IE_Pressed, this, &AMatchPlayerControllerBase::PressTabMenu);
	InputComponent->BindAction("TabMenu", IE_Released, this, &AMatchPlayerControllerBase::ReleasedTabMenu);

	InputComponent->BindAction("SpecialShop", IE_Pressed, this, &AMatchPlayerControllerBase::PressSpecialShop);

	InputComponent->BindAction("ToggleHUD", IE_Released, this, &AMatchPlayerControllerBase::ToggleMainCharacterHUD);
	InputComponent->BindAction("Pause", IE_Released, this, &AMatchPlayerControllerBase::PausePressed);

	InputComponent->BindAction("WorldChat", IE_Pressed, this, &AMatchPlayerControllerBase::CreateChatForAllPlayer);
	InputComponent->BindAction("TeamChat", IE_Pressed, this, &AMatchPlayerControllerBase::CreateChatForAnAlly);
}

void AMatchPlayerControllerBase::CreateChatForAllPlayer()
{
	GetHUD<ABaseMatchHUD>()->ShowChatForAllPlayers();
}

void AMatchPlayerControllerBase::CreateChatForAnAlly()
{
	GetHUD<ABaseMatchHUD>()->ShowChatForAnAlly();
}

void AMatchPlayerControllerBase::OnRep_Pawn()
{
	Super::OnRep_Pawn();
	
	if(GetLocalRole() != ROLE_Authority)
	{
		OnNewPawn.Broadcast(GetPawn());
	}
}

void AMatchPlayerControllerBase::PressTabMenu()
{
	GetHUD<ABaseMatchHUD>()->ShowTabMenu();
}

void AMatchPlayerControllerBase::ReleasedTabMenu()
{
	GetHUD<ABaseMatchHUD>()->HiddenTabMenu();
}

void AMatchPlayerControllerBase::Client_SpecialObjectErrorSpawned_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("ErrorSpawnedSpecialObject"));
}

void AMatchPlayerControllerBase::Server_CreateSpecialObject_Implementation(const FName& ObjectId, const FTransform& Transform)
{
	if(!SpecialObjectManager->CreateSpecialObject(ObjectId, Transform, this))
	{
		Client_SpecialObjectErrorSpawned();
	}
}

bool AMatchPlayerControllerBase::OwnerAddSpecialObject(const FName& ObjectId)
{
	Server_CreateSpecialObject(ObjectId, GetPawn()->GetTransform());
	return true;
}

void AMatchPlayerControllerBase::DecreaseSpecialPoint_Implementation(int32 const Value)
{
	SpecialObjectManager->DecreasePoints(Value);
}

void AMatchPlayerControllerBase::IncreaseSpecialPoint_Implementation(int32 const Value)
{
	SpecialObjectManager->IncreasePoints(Value);
}

void AMatchPlayerControllerBase::PressSpecialShop()
{
	GetHUD<ABaseMatchHUD>()->ShowSpecialWidget();
}

bool AMatchPlayerControllerBase::CheckKeyByName_Implementation(const FName Name)
{
	for(auto& ByArray : InputComponent->KeyBindings)
	{
		if(ByArray.Chord.Key.GetFName() == Name) return true;
	}
	return false;
}

void AMatchPlayerControllerBase::Server_SendMessageFromAllPlayers_Implementation(const FString& Message)
{
	ChatComponent->SendMessageAllPlayer(Message, this);
}

void AMatchPlayerControllerBase::Server_SendMessageFromAnAlly_Implementation(const FString& Message)
{
	ChatComponent->SendMessageFromAnAlly(Message, this);
}

void AMatchPlayerControllerBase::GetMessage_Implementation(const FString& Message, bool const IsOnlyAlly)
{
	Client_GetMessage(Message, IsOnlyAlly);
}

void AMatchPlayerControllerBase::Client_GetMessage_Implementation(const FString& Message, bool const IsOnlyAlly)
{
	OnGetMessage.Broadcast(Message, IsOnlyAlly);
}

void AMatchPlayerControllerBase::ToggleMainCharacterHUD()
{
	if(!GetCharacter()) return;

	auto const HUD = GetHUD<ABaseMatchHUD>();

	if(!HUD || !HUD->GetMainWidget()) return;

	HUD->GetMainWidget()->SetVisibility(HUD->GetMainWidget()->IsVisible() ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
}

void AMatchPlayerControllerBase::PausePressed()
{
	OnPausePressed.Execute();
}

void AMatchPlayerControllerBase::UseAmmunitionState_Implementation()
{
	GetHUD<ABaseMatchHUD>()->CreateAmmunitionWidget();
}

void AMatchPlayerControllerBase::StopUseAmmunitionState()
{
	GetHUD<ABaseMatchHUD>()->RemoveAmmunitionWidget();
}

FString AMatchPlayerControllerBase::PlayerLoginFromController_Implementation()
{
	auto const GameInstance = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	return GameInstance ? GameInstance->GetPlayerName() : "";
}

void AMatchPlayerControllerBase::EndMatch(const FString& Reason, ETeam WinnerTeam)
{
	Client_ConnectToHUBServer();
}

void AMatchPlayerControllerBase::ForcedDisconnectFromServer()
{
	Client_ConnectToHUBServer();
}

void AMatchPlayerControllerBase::Client_ConnectToHUBServer_Implementation()
{
	FString const Address = GetGameInstance<UBaseGameInstance>()->GetCurrentMainHUBServerName();
	UGameplayStatics::OpenLevel(GetWorld(), *Address);
}