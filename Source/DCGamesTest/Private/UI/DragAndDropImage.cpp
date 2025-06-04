// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DragAndDropImage.h"

void UDragAndDropImage::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (MyImage.IsValid())
	{
		MyImage->SetOnMouseButtonUp(BIND_UOBJECT_DELEGATE(FPointerEventHandler, HandleMouseButtonUp));
		MyImage->SetOnMouseMove(BIND_UOBJECT_DELEGATE(FPointerEventHandler, HandleMouseMove));
	}

	if (!OnMouseButtonDownEvent.IsBound()) OnMouseButtonDownEvent.BindUFunction(this, "OnMouseButtonDown");
	if (!OnMouseMoveEvent.IsBound()) OnMouseMoveEvent.BindUFunction(this, "OnMouseMove");
	if (!OnMouseButtonUpEvent.IsBound()) OnMouseButtonUpEvent.BindUFunction(this, "OnMouseButtonUp");

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

FVector2D UDragAndDropImage::AbsoluteToLocalTranslation(const FGeometry& Geometry, const FVector2D& AbsoluteStart, const FVector2D& AbsoluteEnd)
{
	return
		Geometry.AbsoluteToLocal(AbsoluteEnd) -
		Geometry.AbsoluteToLocal(AbsoluteStart);
}

FEventReply UDragAndDropImage::OnMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
	if (!RootWidgetToDrag) RootWidgetToDrag = this;

	bIsBeingDragged = true;
	LocalCursorOffset = Geometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition()) - Geometry.GetLocalSize() * 0.5f;

	return FEventReply(true);
}

FEventReply UDragAndDropImage::OnMouseMove(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
	if (!bIsBeingDragged) return FEventReply(true);
	if (MouseEvent.GetCursorDelta().IsNearlyZero()) return FEventReply(true);

	if (!RootWidgetToDrag) RootWidgetToDrag = this;
	FWidgetTransform NewTransform = RootWidgetToDrag->GetRenderTransform();

	FVector2D LocalSize = Geometry.GetLocalSize();
	FVector2D LocalTranslation = AbsoluteToLocalTranslation(Geometry, MouseEvent.GetLastScreenSpacePosition(), MouseEvent.GetScreenSpacePosition());

	if (!bLockTranslation)
	{
		NewTransform.Translation += LocalTranslation.GetRotated(NewTransform.Angle);
	}

	if (!bLockRotation)
	{
		FVector2D LocalCursorPosition = Geometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
		FVector2D LocalCursorToOldPosition = (LocalSize * 0.5f - LocalCursorPosition).GetSafeNormal();
		FVector2D LocalCursorToNewPosition = (LocalSize * 0.5f + LocalTranslation - LocalCursorPosition).GetSafeNormal();

		float DeltaAngle = FMath::RadiansToDegrees(FMath::Acos(LocalCursorToOldPosition | LocalCursorToNewPosition)) * FMath::Sign(LocalCursorToNewPosition ^ LocalCursorToOldPosition);
		FVector2D LocalCorrection = LocalCursorPosition - (LocalSize * 0.5f + LocalTranslation + LocalCursorOffset.GetRotated(DeltaAngle));

		NewTransform.Angle += DeltaAngle;
		NewTransform.Translation += LocalCorrection.GetRotated(NewTransform.Angle);
	}

	RootWidgetToDrag->SetRenderTransform(NewTransform);

	return FEventReply(true);
}

FEventReply UDragAndDropImage::OnMouseButtonUp(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
	bIsBeingDragged = false;

	return FEventReply(true);
}

bool UDragAndDropImage::IsBeingDragged()
{
	return bIsBeingDragged;
}