// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseMatchHUD.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SpaceWar/PlayerControllers/Match/Base/MatchPlayerControllerBase.h"
#include "SpaceWar/PlayerStart/MatchPlayerStartBase.h"
#include "UObject/ConstructorHelpers.h"

ABaseMatchHUD::ABaseMatchHUD()
{
	MatchType = EMatchData::CaptureOfFlag;
	
	ConstructorHelpers::FObjectFinder<UMatchWidgetDataAsset>WidgetData(TEXT("/Game/ThirdPersonCPP/DataAssets/WidgetDataAsset"));
	if(WidgetData.Succeeded()) AssetData = WidgetData.Object;
	
	ConstructorHelpers::FClassFinder<UErrorMessageWidget>ErrorMessageFinder(TEXT("/Game/ThirdPersonCPP/UI/ErrorMessage/W_ErrorMessage"));
	if(ErrorMessageFinder.Succeeded()) ErrorWidgetClass = ErrorMessageFinder.Class;

	ConstructorHelpers::FClassFinder<UMatchChatWidgetBase>ChatWidgetFinder(TEXT("/Game/ThirdPersonCPP/UI/Matches/Chat/W_MatchChat"));
	if(ChatWidgetFinder.Succeeded()) ChatWidgetClass = ChatWidgetFinder.Class;

	ConstructorHelpers::FClassFinder<UAmmunitionWidgetBase>AmmunitionWidgetFinder(TEXT("/Game/ThirdPersonCPP/UI/AmmunitionState/W_AmmuniitonGrid"));
	if(AmmunitionWidgetFinder.Succeeded()) AmmunitionWidgetClass = AmmunitionWidgetFinder.Class;
}

void ABaseMatchHUD::BeginPlay()
{
	Super::BeginPlay();

	/** set widget struct */
	MatchWidgetData = AssetData->GetWidgetData(MatchType);

	/** create dynamic widget */
	CreateTabMenu();
	CreateChatWidget();

	/** bind on new player connected */
	GetOwningPlayerController()->GetOnNewPawnNotifier().AddUObject(this, &ABaseMatchHUD::NewOwningPlayerPawn);
	Cast<AMatchPlayerControllerBase>(GetOwningPlayerController())->OnPausePressed.AddDynamic(this, &ABaseMatchHUD::PausePressed);
}

void ABaseMatchHUD::ClientErrorMessage_Implementation(const FString& Message)
{
	CreateErrorWidget(Message);
}

void ABaseMatchHUD::CreateErrorWidget(const FString& Message)
{
	auto const Widget = AssetData->SyncCreateWidget<UErrorMessageWidget>(GetWorld(), ErrorWidgetClass, GetOwningPlayerController());
	if(Widget)
	{
		Widget->Init(Message);
		Widget->AddToViewport();
		
		auto AutoDestroy = [&]() -> void { Widget->RemoveFromParent(); };
		
		FTimerHandle TimerHandle;
		FTimerDelegate TimerDel;
		TimerDel.BindLambda(AutoDestroy);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, 2.f, false);
	}
}

void ABaseMatchHUD::NewOwningPlayerPawn(APawn* NewPawn)
{
	if(GetOwningPlayerController()->GetCharacter())
	{
		//Character
		CreateCharacterWidgets();
		CreateSpecialWidget();
	}
	else
	{
		//Spectator
		RemoveCharacterWidgets();
		RemoveSpecialWidget();
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
	if(MatchWidgetData->SpecialShop)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Create!!!!!!!!"));
		SpecialShopWidget = AssetData->SyncCreateWidget<UUserWidget>(GetWorld(), MatchWidgetData->SpecialShop, GetOwningPlayerController());
		SpecialShopWidget->AddToViewport();
		SpecialShopWidget->SetVisibility(ESlateVisibility::Collapsed);
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
	PreparationWidget = AssetData->SyncCreateWidget<UUserWidget>(GetWorld(), MatchWidgetData->PreparationMatch, GetOwningPlayerController());
	PreparationWidget->AddToViewport();
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

void ABaseMatchHUD::PausePressed()
{
	if(!ChatWidget->IsHidden())
	{
		ChatWidget->HiddenChat();
		return;
	}
	/** show pause */
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
