// Fill out your copyright notice in the Description page of Project Settings.


#include "MatchChatWidgetBase.h"
#include "Components/PanelSlot.h"
#include "Components/EditableText.h"

void UMatchChatWidgetBase::ShowChat_Implementation(bool bOnlyTeam)
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	bHidden = false;
}

void UMatchChatWidgetBase::HiddenChat_Implementation()
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	bHidden = true;
}

void UMatchChatWidgetBase::AutoHiddenMessage(UWidget* MessageWidget, UBorder* Border)
{
	FTimerDelegate TimerDel;
	TimerDel.BindUObject(this, &UMatchChatWidgetBase::HiddenMessage, MessageWidget, Border);
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, 3.5f, false);
}

void UMatchChatWidgetBase::HiddenMessage(UWidget* MessageWidget, UBorder* Border)
{
	if(Border->IsVisible()) return;
	MessageWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void UMatchChatWidgetBase::ShowAllMessage(UScrollBox* ScrollBox)
{
	for(const auto& ByArray : ScrollBox->GetAllChildren())
	{
		ByArray->SetVisibility(ESlateVisibility::Visible);
	}
}

void UMatchChatWidgetBase::HiddenAllMessage(UScrollBox* ScrollBox)
{
	for(const auto& ByArray : ScrollBox->GetAllChildren())
	{
		ByArray->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UMatchChatWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	bHidden = true;
}

void UMatchChatWidgetBase::NativeOnFocusChanging(const FWeakWidgetPath& PreviousFocusPath, const FWidgetPath& NewWidgetPath, const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusChanging(PreviousFocusPath, NewWidgetPath, InFocusEvent);
	
	if((InFocusEvent.GetCause() == EFocusCause::Cleared || InFocusEvent.GetCause() == EFocusCause::Mouse) && !bHidden)
	{
		HiddenChat();
	}
}
