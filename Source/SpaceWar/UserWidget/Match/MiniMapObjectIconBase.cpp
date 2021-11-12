// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniMapObjectIconBase.h"

#include "Kismet/KismetMaterialLibrary.h"
#include "SpaceWar/HUD/Match/BaseMatchHUD.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "SpaceWar/Interfaces/MiniMapInterface.h"

void UMiniMapObjectIconBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(bUpdate)
	{
		UpdateObjectLocation();
	}
}

void UMiniMapObjectIconBase::GetXForUpdateLocation(float& ObjectX, float& OwnerX)
{
	ObjectX = GetObjectIconOwner()->GetActorLocation().X;
	OwnerX = GetOwningPlayerPawn()->GetActorLocation().X;
}

void UMiniMapObjectIconBase::GetYForUpdateLocation(float& ObjectY, float& OwnerY)
{
	ObjectY = GetObjectIconOwner()->GetActorLocation().Y;
	OwnerY = GetOwningPlayerPawn()->GetActorLocation().Y;
}

FVector2D UMiniMapObjectIconBase::CalculatePosition(UMiniMapBase* MiniMap, float MaxIconDistance)
{
	FVector2D const DesiredSize = MiniMap->GetMapImage()->GetDesiredSize();

	float const LiteralX = UKismetSystemLibrary::MakeLiteralFloat(DesiredSize.X);
	float const LiteralY = UKismetSystemLibrary::MakeLiteralFloat(DesiredSize.Y);

	float OwnerX, ObjectX;
	float OwnerY, ObjectY;
	GetXForUpdateLocation(ObjectX, OwnerX);
	GetYForUpdateLocation(ObjectY, OwnerY);

	float const MaterialParam = UKismetMaterialLibrary::GetScalarParameterValue(GetWorld(), MaterialParameterCollection, "Zoom");

	float const CalculateX = (OwnerX - ObjectX) / (MaterialParam * (MiniMap->GetDevidion() / LiteralX));
	float const CalculateY = (OwnerY - ObjectY) / (MaterialParam * (MiniMap->GetDevidion() / LiteralY));

	float const LocationLenght =  FVector2D(CalculateX, CalculateY).Size();

	float const AtanLocation = (180.f) / PI * FMath::Atan2(CalculateY * (-1), CalculateX * (-1));
	float const ClampLocationLenght = FMath::Clamp(LocationLenght, 0.f, MaxIconDistance);

	float const SinD = FMath::Sin(PI / (180.f) * AtanLocation);
	float const CosD = FMath::Cos(PI / (180.f) * AtanLocation);

	return FVector2D(SinD * ClampLocationLenght, (CosD * ClampLocationLenght) * -1);
}

void UMiniMapObjectIconBase::UpdateObjectLocation()
{
	if(!GetOwningPlayer() || !GetObjectIconOwner() || !GetOwningPlayerPawn())
	{
		bUpdate = false;
		RemoveFromParent();
		return;
	}
	
	auto const MainWidget = GetOwningPlayer()->GetHUD<ABaseMatchHUD>()->GetMainWidget();
	if(!MainWidget || !MainWidget->GetClass()->ImplementsInterface(UMiniMapInterface::StaticClass()))
	{
		bUpdate = false;
		RemoveFromParent();
		return;
	}
	
	auto const MiniMap = IMiniMapInterface::Execute_GetMiniMapWidget(MainWidget);

	if(!MiniMap) RemoveFromParent();

	auto const DesiredSize = MiniMap->GetMapImage()->GetDesiredSize();

	float const MaxIconDistance = DesiredSize.X / 2 * 0.9f;

	FVector2D const Position = CalculatePosition(MiniMap, MaxIconDistance);

	SetRenderTranslation(Position);

	if(!bHasConst)
	{
		SetVisibility((Position.Size() >= MaxIconDistance - 1) ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	}
}
