// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMatchHUD.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UObject/ConstructorHelpers.h"

ABaseMatchHUD::ABaseMatchHUD()
{
	MatchType = EMatchData::CaptureOfFlag;
	
	ConstructorHelpers::FObjectFinder<UMatchWidgetDataAsset>WidgetData(TEXT("/Game/ThirdPersonCPP/DataAssets/WidgetDataAsset"));
	if(WidgetData.Succeeded()) AssetData = WidgetData.Object;
	
	ConstructorHelpers::FClassFinder<UErrorMessageWidget>ErrorMessageFinder(TEXT("/Game/ThirdPersonCPP/UI/ErrorMessage/W_ErrorMessage"));
	if(ErrorMessageFinder.Succeeded()) ErrorWidgetClass = ErrorMessageFinder.Class;
}

void ABaseMatchHUD::BeginPlay()
{
	Super::BeginPlay();

	MatchWidgetData = AssetData->GetWidgetData(MatchType);
	
	CreateTabMenu();
	GetOwningPlayerController()->GetOnNewPawnNotifier().AddUObject(this, &ABaseMatchHUD::NewOwningPlayerPawn);
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
		auto AutoDestroy = [&]() -> void
		{
			Widget->RemoveFromParent();
		};
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

