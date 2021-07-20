// Fill out your copyright notice in the Description page of Project Settings.


#include "MatchPlayerControllerBase.h"
#include "Blueprint/UserWidget.h"
#include "SpaceWar/HUD/Match/BaseMatchHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "UObject/ConstructorHelpers.h"
#include "SpaceWar/Actors/Match/SpecialWeapon/SpecialWeaponObjectBase.h"
#include "SpaceWar/SpaceWarCharacter.h"
#include "SpaceWar/GameModes/Match/Base/MatchGameModeBase.h"

AMatchPlayerControllerBase::AMatchPlayerControllerBase()
{
	bReplicates = true;
	NetUpdateFrequency = 2.f;

	DecreaseSpecialPointsValue = 200;
	bAttachToPawn = true;

	SpecialObjectManager = CreateDefaultSubobject<USpecialObjectManagerComponent>(TEXT("SpecialObjectManager"));

	static ConstructorHelpers::FClassFinder<ASpaceWarCharacter> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if(PlayerPawnBPClass.Class != nullptr)
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
	auto const GM = Cast<AMatchGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if(GM)
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
	OnPausePressed.Broadcast();
}

void AMatchPlayerControllerBase::UseAmmunitionState_Implementation()
{
	
}
