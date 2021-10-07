// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MiniMapObjectIconBase.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API UMiniMapObjectIconBase : public UUserWidget
{
	GENERATED_BODY()

	FVector2D CalculatePosition(class UMiniMapBase* MiniMap, float MaxIconDistance);
	void GetXForUpdateLocation(float& ObjectX, float& OwnerX);
	void GetYForUpdateLocation(float& ObjectY, float& OwnerY);

protected:

	UFUNCTION(BlueprintImplementableEvent)
    AActor* GetObjectIconOwner();

public:

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:

	UFUNCTION(BlueprintCallable)
	void UpdateObjectLocation();

	UFUNCTION(BlueprintImplementableEvent)
	float GetScalarParameterZoom();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bHasConst;

	UPROPERTY(EditAnywhere)
	UMaterialParameterCollection* MaterialParameterCollection;
	
	UPROPERTY(BlueprintReadWrite)
	bool bUpdate;
};
