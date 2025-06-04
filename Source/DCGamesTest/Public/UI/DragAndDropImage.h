// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Input/Reply.h"

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "DragAndDropImage.generated.h"

UENUM(BlueprintType)
enum class EDragAxis : uint8
{
	DA_XAxis	UMETA(DisplayName = "X"),
	DA_YAxis	UMETA(DisplayName = "Y"),
	DA_XYAxes	UMETA(DisplayName = "XY")
};

/**
 * 
 */
UCLASS()
class DCGAMESTEST_API UDragAndDropImage : public UImage
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = Events, meta = (IsBindableEvent = "True"))
	FOnPointerEvent OnMouseMoveEvent;

	UPROPERTY(EditAnywhere, Category = Events, meta = (IsBindableEvent = "True"))
	FOnPointerEvent OnMouseButtonUpEvent;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drag And Drop")
	bool bLockTranslation = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drag And Drop")
	bool bLockRotation = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drag And Drop")
	EDragAxis AllowedDragAxes = EDragAxis::DA_XYAxes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drag And Drop")
	TObjectPtr<UWidget> RootWidgetToDrag;

protected:
	bool bIsBeingDragged = false;

	FVector2D LocalCursorOffset;

protected:
	virtual void SynchronizeProperties() override;

protected:
	FReply HandleMouseButtonUp(const FGeometry& Geometry, const FPointerEvent& MouseEvent);
	FReply HandleMouseMove(const FGeometry& Geometry, const FPointerEvent& MouseEvent);

	FVector2D AbsoluteToLocalTranslation(const FGeometry& Geometry, const FVector2D& AbsoluteStart, const FVector2D& AbsoluteEnd);

protected:
	UFUNCTION(BlueprintCallable)
	FEventReply OnMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& MouseEvent);

	UFUNCTION(BlueprintCallable)
	FEventReply OnMouseMove(const FGeometry& Geometry, const FPointerEvent& MouseEvent);

	UFUNCTION(BlueprintCallable)
	FEventReply OnMouseButtonUp(const FGeometry& Geometry, const FPointerEvent& MouseEvent);

public:
	UFUNCTION(BlueprintPure)
	bool IsBeingDragged();
};
