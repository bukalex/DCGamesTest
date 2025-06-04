// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/FlashDriveWidget.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FlashDriveMiniWidget.generated.h"

/**
 * 
 */
UCLASS()
class DCGAMESTEST_API UFlashDriveMiniWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, meta = (AllowedClasses = "UFlashDriveWidget"))
	TArray<TSubclassOf<UUserWidget>> FlashDriveMinigames;

protected:
	TObjectPtr<UFlashDriveWidget> DisplayedMinigame;

protected:
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

protected:
	UFUNCTION()
	void OnDriveUnlocked();
};
