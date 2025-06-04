// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FlashDriveWidget.h"

#include "Components/CanvasPanel.h"

void UFlashDriveWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!bIsLocked)
	{
		if (bIsGrowing)
		{
			FD_CanvasPanel->SetRenderScale(FMath::Vector2DInterpTo(FD_CanvasPanel->GetRenderTransform().Scale, TargetScale, InDeltaTime, ScalingSpeed));

			if ((FD_CanvasPanel->GetRenderTransform().Scale - TargetScale).IsNearlyZero(0.01f))
			{
				bIsGrowing = false;
			}
		}
		else
		{
			FD_CanvasPanel->SetRenderScale(FMath::Vector2DInterpTo(FD_CanvasPanel->GetRenderTransform().Scale, FVector2D::UnitVector, InDeltaTime, ScalingSpeed));

			if ((FD_CanvasPanel->GetRenderTransform().Scale - FVector2D::UnitVector).IsNearlyZero(0.01f))
			{
				OnDriveUnlocked.Broadcast();
			}
		}
	}
}

void UFlashDriveWidget::UnlockDrive()
{
	bIsLocked = false;
}