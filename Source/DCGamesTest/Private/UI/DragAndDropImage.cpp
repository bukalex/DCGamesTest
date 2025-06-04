// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DragAndDropImage.h"

void UDragAndDropImage::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (MyImage.IsValid())
	{
		MyImage->SetOnMouseButtonUp(BIND_UOBJECT_DELEGATE(FPointerEventHandler, HandleMouseButtonUp));
		MyImage->SetOnMouseMove(BIND_UOBJECT_DELEGATE(FPointerEventHandler, HandleMouseMove));
		MyImage->SetOnMouseLeave(BIND_UOBJECT_DELEGATE(FSimpleNoReplyPointerEventHandler, HandleMouseLeave));
	}

	if (!OnMouseButtonDownEvent.IsBound()) OnMouseButtonDownEvent.BindUFunction(this, "OnMouseButtonDown");
	if (!OnMouseMoveEvent.IsBound()) OnMouseMoveEvent.BindUFunction(this, "OnMouseMove");
	if (!OnMouseButtonUpEvent.IsBound()) OnMouseButtonUpEvent.BindUFunction(this, "OnMouseButtonUp");
	if (!OnMouseLeaveEvent.IsBound()) OnMouseLeaveEvent.BindUFunction(this, "OnMouseLeave");

	if (!RootWidgetToDrag) RootWidgetToDrag = this;
}

FReply UDragAndDropImage::HandleMouseButtonUp(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
	if (OnMouseButtonUpEvent.IsBound())
	{
		return OnMouseButtonUpEvent.Execute(Geometry, MouseEvent).NativeReply;
	}

	return FReply::Unhandled();
}

FReply UDragAndDropImage::HandleMouseMove(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
	if (OnMouseMoveEvent.IsBound())
	{
		return OnMouseMoveEvent.Execute(Geometry, MouseEvent).NativeReply;
	}

	return FReply::Unhandled();
}

void UDragAndDropImage::HandleMouseLeave(const FPointerEvent& MouseEvent)
{
	if (OnMouseLeaveEvent.IsBound())
	{
		OnMouseLeaveEvent.Execute(MouseEvent);
	}
}

void UDragAndDropImage::ClampTranslationToLocalAxis(FVector2D& LocalTranslation)
{
	switch (AllowedDragAxes)
	{
	case EDragAxis::DA_XAxis:
		LocalTranslation.Y = 0;
		break;

	case EDragAxis::DA_YAxis:
		LocalTranslation.X = 0;
		break;
	}
}

FEventReply UDragAndDropImage::OnMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
	if (!RootWidgetToDrag) RootWidgetToDrag = this;
	const FGeometry RootGeometry = RootWidgetToDrag->GetCachedGeometry();

	FVector2D LocalSize = RootGeometry.GetLocalSize();
	LocalPivotPosition = FVector2D(LocalSize.X * RootWidgetToDrag->GetRenderTransformPivot().X, LocalSize.Y * RootWidgetToDrag->GetRenderTransformPivot().Y);

	bIsBeingDragged = true;
	LocalCursorOffset = RootGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition()) - LocalPivotPosition;

	return FEventReply(true);
}

FEventReply UDragAndDropImage::OnMouseMove(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
	if (!bIsBeingDragged) return FEventReply(true);
	if (MouseEvent.GetCursorDelta().IsNearlyZero()) return FEventReply(true);

	if (!RootWidgetToDrag) RootWidgetToDrag = this;
	const FGeometry RootGeometry = RootWidgetToDrag->GetCachedGeometry();
	FWidgetTransform OldTransform = RootWidgetToDrag->GetRenderTransform();
	FWidgetTransform NewTransform = RootWidgetToDrag->GetRenderTransform();

	FVector2D LocalTranslation =
		RootGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition()) -
		RootGeometry.AbsoluteToLocal(MouseEvent.GetLastScreenSpacePosition());
	ClampTranslationToLocalAxis(LocalTranslation);

	if (!bLockTranslation)
	{
		NewTransform.Translation += LocalTranslation.GetRotated(NewTransform.Angle);
	}

	if (!bLockRotation && AllowedDragAxes == EDragAxis::DA_XYAxes)
	{
		FVector2D LocalCursorPosition = RootGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
		FVector2D LocalCursorToOldPosition = (LocalPivotPosition - LocalCursorPosition).GetSafeNormal();
		FVector2D LocalCursorToNewPosition = (LocalPivotPosition + LocalTranslation - LocalCursorPosition).GetSafeNormal();

		float DeltaAngle = FMath::RadiansToDegrees(FMath::Acos(LocalCursorToOldPosition | LocalCursorToNewPosition)) * FMath::Sign(LocalCursorToNewPosition ^ LocalCursorToOldPosition);
		NewTransform.Angle += DeltaAngle;

		if (!bLockTranslation)
		{
			FVector2D LocalCorrection = LocalCursorPosition - (LocalPivotPosition + LocalTranslation + LocalCursorOffset.GetRotated(DeltaAngle));
			NewTransform.Translation += LocalCorrection.GetRotated(NewTransform.Angle);
		}
	}

	RootWidgetToDrag->SetRenderTransform(NewTransform);
	if (OnWidgetMoved.IsBound()) OnWidgetMoved.Execute(OldTransform, NewTransform);

	return FEventReply(true);
}

FEventReply UDragAndDropImage::OnMouseButtonUp(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
	bIsBeingDragged = false;

	return FEventReply(true);
}

void UDragAndDropImage::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	bIsBeingDragged = false;
}

bool UDragAndDropImage::IsBeingDragged()
{
	return bIsBeingDragged;
}