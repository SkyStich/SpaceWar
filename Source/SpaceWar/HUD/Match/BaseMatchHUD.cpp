// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseMatchHUD.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SpaceWar/GameStates/Match/OnlinetMatchGameStateBase.h"
#include "SpaceWar/PlayerControllers/Match/Base/MatchPlayerControllerBase.h"
#include "SpaceWar/PlayerStart/MatchPlayerStartBase.h"
#include "UObject/ConstructorHelpers.h"

ABaseMatchHUD::ABaseMatchHUD()
{
	MatchType = EMatchData::CaptureOfFlag;
	
	static ConstructorHelpers::FObjectFinder<UMatchWidgetDataAsset>WidgetData(TEXT("/Game/ThirdPersonCPP/DataAssets/WidgetDataAsset"));
	if(WidgetData.Succeeded()) AssetData = WidgetData.Object;
	
	static ConstructorHelpers::FClassFinder<UErrorMessageWidget>ErrorMessageFinder(TEXT("/Game/ThirdPersonCPP/UI/ErrorMessage/W_ErrorMessage"));
	if(ErrorMessageFinder.Succeeded()) ErrorWidgetClass = ErrorMessageFinder.Class;

	static ConstructorHelpers::FClassFinder<UMatchChatWidgetBase>ChatWidgetFinder(TEXT("/Game/ThirdPersonCPP/UI/Matches/Chat/W_MatchChat"));
	if(ChatWidgetFinder.Succeeded()) ChatWidgetClass = ChatWidgetFinder.Class;

	static ConstructorHelpers::FClassFinder<UAmmunitionWidgetBase>AmmunitionWidgetFinder(TEXT("/Game/ThirdPersonCPP/UI/AmmunitionState/W_AmmuniitonGrid"));
	if(AmmunitionWidgetFinder.Succeeded()) AmmunitionWidgetClass = AmmunitionWidgetFinder.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget>SpecialShopWidgetFinder(TEXT("/Game/ThirdPersonCPP/UI/Matches/LastWidget/W_ShopSpecialWeapon"));
	if(SpecialShopWidgetFinder.Succeeded()) SpecialShopWidgetClass = SpecialShopWidgetFinder.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget>SpectatorWidgetFinder(TEXT("/Game/ThirdPersonCPP/UI/Matches/LastWidget/Spectator/W_MainSpectator"));
	if(SpectatorWidgetFinder.Succeeded()) SpectatorWidget = SpectatorWidgetFinder.Class;
	
	static ConstructorHelpers::FClassFinder<UUserWidget>KillMessageFinder(TEXT("/Game/ThirdPersonCPP/UI/Matches/LastWidget/KillChat/W_KillChat"));
	if(KillMessageFinder.Succeeded()) KillMessageClass = KillMessageFinder.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget>PauseMenuFinder(TEXT("/Game/ThirdPersonCPP/UI/Matches/PauseMenu/W_PauseMenu"));
	if(PauseMenuFinder.Succeeded()) PauseWidgetClass = PauseMenuFinder.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> LevelGridFinder(TEXT("/Game/ThirdPersonCPP/UI/ExpLogic/W_LevelGrid"));
	if(LevelGridFinder.Succeeded()) LevelGridClass = LevelGridFinder.Class;
}

void ABaseMatchHUD::BeginPlay()
{
	Super::BeginPlay();

	/** set widget struct */
	MatchWidgetData = AssetData->GetWidgetData(MatchType);

	/** create dynamic widget */
	CreateTabMenu();
	CreateChatWidget();
	CreateKillMessage();

	auto const GS = Cast<AOnlinetMatchGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	GS->OnPreparationStartGameFinish.AddDynamic(this, &ABaseMatchHUD::OnPreparationStartGameEvent);
	
	Cast<AMatchPlayerControllerBase>(GetOwningPlayerController())->OnPausePressed.BindUFunction(this, "OnPausePressed");
}

UErrorMessageWidget* ABaseMatchHUD::ClientErrorMessage_Implementation(const FString& Message)
{
	return CreateErrorWidget(Message);
}

UErrorMessageWidget* ABaseMatchHUD::CreateErrorWidget(const FString& Message)
{
	auto const Widget = AssetData->SyncCreateWidget<UErrorMessageWidget>(GetWorld(), ErrorWidgetClass, GetOwningPlayerController());
	if(Widget)
	{
		Widget->Init(Message);
		Widget->AddToViewport();
	}
	return Widget;
}

void ABaseMatchHUD::NewOwningPlayerPawn(APawn* NewPawn)
{
	if(GetOwningPlayerController()->GetCharacter())
	{
		//Character
		RemovePreparationWidget();
		CreateCharacterWidgets();
		CreateSpecialWidget();
	}
	else
	{
		//Spectator
		RemoveCharacterWidgets();
		RemoveSpecialWidget();
		RemoveAmmunitionWidget();
		CreateSpectatorWidgets();
	}
}

void ABaseMatchHUD::CreateTabMenu()
{
	TabMenuWidget = AssetData->SyncCreateWidget<UUserWidget>(GetWorld(), MatchWidgetData->StatisticMatchGrid, GetOwningPlayerController());
	TabMenuWidget->AddToViewport();
	TabMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void ABaseMatchHUD::CreateCharacterWidgets()
{
	if(!CharacterHUD)
	{
		CharacterHUD = AssetData->SyncCreateWidget<UUserWidget>(GetWorld(), MatchWidgetData->HUD, GetOwningPlayerController());
		CharacterHUD->AddToViewport();	
	}
}

void ABaseMatchHUD::RemoveCharacterWidgets()
{
	if(CharacterHUD)
	{
		CharacterHUD->RemoveFromParent();
		CharacterHUD = nullptr;
	}
}

void ABaseMatchHUD::CreateSpectatorWidgets()
{
	if(SpectatorWidget && !SpectatorHUD && !PreparationWidget)
	{
		SpectatorHUD = AssetData->SyncCreateWidget<UUserWidget>(GetWorld(), SpectatorWidget, GetOwningPlayerController());
		SpectatorHUD->AddToViewport();
	}
}

void ABaseMatchHUD::RemoveSpectatorWidgets()
{
	if(SpectatorHUD)
	{
		SpectatorHUD->RemoveFromParent();
		SpectatorHUD = nullptr;
	}
}

void ABaseMatchHUD::ShowTabMenu()
{
	TabMenuWidget->SetVisibility(ESlateVisibility::Visible);
}

void ABaseMatchHUD::HiddenTabMenu()
{
	TabMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void ABaseMatchHUD::CreateSpecialWidget()
{
	if(SpecialShopWidgetClass)
	{
		SpecialShopWidget = AssetData->SyncCreateWidget<UUserWidget>(GetWorld(), SpecialShopWidgetClass, GetOwningPlayerController());
		SpecialShopWidget->AddToViewport();
	}
}

void ABaseMatchHUD::RemoveSpecialWidget()
{
	if(SpecialShopWidget)
	{
		SpecialShopWidget->RemoveFromParent();
		SpecialShopWidget = nullptr;
	}
}

void ABaseMatchHUD::ShowSpecialWidget()
{
	if(SpecialShopWidget && SpecialShopWidget->GetVisibility() != ESlateVisibility::Visible)
	{
		GetOwningPlayerController()->bShowMouseCursor = true;
		SpecialShopWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ABaseMatchHUD::CreatePreparationWidget()
{
	if(!PreparationWidget)
	{
		PreparationWidget = AssetData->SyncCreateWidget<UUserWidget>(GetWorld(), MatchWidgetData->PreparationMatch, GetOwningPlayerController());
		PreparationWidget->AddToViewport();
	}
}

void ABaseMatchHUD::RemovePreparationWidget()
{
	if(PreparationWidget)
	{
		PreparationWidget->RemoveFromParent();
		PreparationWidget = nullptr;
		FInputModeGameOnly const InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->bShowMouseCursor = false;
	}
}

void ABaseMatchHUD::CreatePreMatchEnd(const FString& Reason, ETeam WinnerTeam)
{
	PreEndMatchWidget = AssetData->SyncCreateWidget<UEndGameWidgetBase>(GetWorld(), MatchWidgetData->PreEndMatch, GetOwningPlayerController());
	PreEndMatchWidget->Init(Reason, WinnerTeam);
	PreEndMatchWidget->AddToViewport();

	LevelGridWidget = AssetData->SyncCreateWidget<UUserWidget>(GetWorld(), LevelGridClass, GetOwningPlayerController());
	LevelGridWidget->AddToViewport(1);
}

void ABaseMatchHUD::RemovePreMatchEnd()
{
	if(PreEndMatchWidget)
	{
		PreEndMatchWidget->RemoveFromParent();
		PreEndMatchWidget = nullptr;
	}
}

void ABaseMatchHUD::CreateMatchEndWidget(const FString& Reason, ETeam WinnerTeam)
{
	if(!EndGameMatchWidget)
	{
		EndGameMatchWidget = AssetData->SyncCreateWidget<UEndGameWidgetBase>(GetWorld(), MatchWidgetData->EndMatch, GetOwningPlayerController());
		EndGameMatchWidget->Init(Reason, WinnerTeam);
		EndGameMatchWidget->AddToViewport();
		RemovePreMatchEnd();
	}
}

void ABaseMatchHUD::RemoveMatchEndWidget()
{
	if(EndGameMatchWidget)
	{
		EndGameMatchWidget->RemoveFromParent();
		EndGameMatchWidget = nullptr;
	}
}

void ABaseMatchHUD::CreateChatWidget()
{
	if(!ChatWidget)
	{
		ChatWidget = AssetData->SyncCreateWidget<UMatchChatWidgetBase>(GetWorld(), ChatWidgetClass, GetOwningPlayerController());
		ChatWidget->AddToViewport(1);
	}
}

void ABaseMatchHUD::ShowChatForAllPlayers()
{
	if(ChatWidget)
	{
		ChatWidget->ShowChat(false);
	}
}

void ABaseMatchHUD::ShowChatForAnAlly()
{
	if(ChatWidget)
	{
		ChatWidget->ShowChat(true);
	}
}

void ABaseMatchHUD::HiddenChat()
{
	if(ChatWidget)
	{
		ChatWidget->HiddenChat();
	}
}

void ABaseMatchHUD::HiddenMainCharacterWidget()
{
	if(CharacterHUD)
	{
		CharacterHUD->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ABaseMatchHUD::ShowMainCharacterWidget()
{
	if(CharacterHUD)
	{
		CharacterHUD->SetVisibility(ESlateVisibility::Visible);
	}
}

void ABaseMatchHUD::CreateAmmunitionWidget()
{
	if(AmmunitionWidgetClass && !AmmunitionWidget)
	{
		AmmunitionWidget = AssetData->SyncCreateWidget<UAmmunitionWidgetBase>(GetWorld(), AmmunitionWidgetClass, GetOwningPlayerController());
		if(AmmunitionWidget)
		{
			AmmunitionWidget->AddToViewport();
			return;
		}
		UE_LOG(LogViewport, Error, TEXT("ABaseMatchHUD::CreateAmmunitionWidget -- Can not crete ammunition widget: %s"), *GetFullName());
	}
}

void ABaseMatchHUD::RemoveAmmunitionWidget()
{
	if(AmmunitionWidget)
	{
		AmmunitionWidget->RemoveFromParent();
		AmmunitionWidget = nullptr;
	}
}

void ABaseMatchHUD::CreateKillMessage()
{
	if(KillMessageClass)
	{
		KillMessageWidget = AssetData->SyncCreateWidget<UUserWidget>(GetWorld(), KillMessageClass, GetOwningPlayerController());
		KillMessageWidget->AddToViewport();
	}
}

void ABaseMatchHUD::RemoveKillMessage()
{
	if(KillMessageWidget)
	{
		KillMessageWidget->RemoveFromParent();
		KillMessageWidget = nullptr;
	}
}

void ABaseMatchHUD::CreatePauseMenu()
{
	if(!PauseWidget)
	{
		PauseWidget = AssetData->SyncCreateWidget<UUserWidget>(GetWorld(), PauseWidgetClass, GetOwningPlayerController());
		PauseWidget->AddToViewport();

		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		InputMode.SetHideCursorDuringCapture(false);
		InputMode.SetWidgetToFocus(PauseWidget->TakeWidget());
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->bShowMouseCursor = true;
	}
}

void ABaseMatchHUD::RemovePauseMenu()
{
	if(PauseWidget)
	{
		PauseWidget->RemoveFromParent();
		PauseWidget = nullptr;

		GetOwningPlayerController()->SetInputMode(FInputModeGameOnly());
		GetOwningPlayerController()->bShowMouseCursor = false;
	}
}

void ABaseMatchHUD::OnPausePressed()
{
	if(!ChatWidget->IsHidden()) return;

	if(SpecialShopWidget && SpecialShopWidget->IsVisible())
	{
		SpecialShopWidget->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	if(AmmunitionWidget)
	{
		AmmunitionWidget->StopUseState();
		return;
	}

	if(PauseWidget)
	{
		RemovePauseMenu();
	}
	else
	{
		CreatePauseMenu();
	}
}