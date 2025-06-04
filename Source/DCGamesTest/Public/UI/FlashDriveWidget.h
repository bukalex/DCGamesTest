// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FlashDriveWidget.generated.h"

class UCanvasPanel;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDriveUnlocked);

/**
 * 
 */
UCLASS()
class DCGAMESTEST_API UFlashDriveWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> FD_CanvasPanel;

	UPROPERTY(EditAnywhere)
	FVector2D TargetScale = FVector2D(1.5f, 1.5f);

	UPROPERTY(EditAnywhere)
	float ScalingSpeed = 5;
	
public:
	FOnDriveUnlocked OnDriveUnlocked;

protected:
	bool bIsGrowing = true;
	bool bIsLocked = true;

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
	void UnlockDrive();
};
