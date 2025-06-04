// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FlashDriveMiniWidget.h"

FReply UFlashDriveMiniWidget::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (FlashDriveMinigames.IsEmpty()) return FReply::Unhandled();
	if (DisplayedMinigame) return FReply::Unhandled();

	TSubclassOf<UUserWidget> RandomSubclass = FlashDriveMinigames[FMath::Rand() % FlashDriveMinigames.Num()];
	if (!RandomSubclass) return FReply::Unhandled();

	DisplayedMinigame = CreateWidget<UFlashDriveWidget>(this, RandomSubclass);
	if (!DisplayedMinigame) return FReply::Unhandled();

	DisplayedMinigame->AddToViewport();
	DisplayedMinigame->OnDriveUnlocked.AddDynamic(this, &UFlashDriveMiniWidget::OnDriveUnlocked);

	return FReply::Handled();
}

void UFlashDriveMiniWidget::OnDriveUnlocked()
{
	DisplayedMinigame->RemoveFromParent();
	DisplayedMinigame->Destruct();
	DisplayedMinigame = nullptr;
}