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

FEventReply UDragAndDropImage::OnMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
	if (!RootWidgetToDrag) RootWidgetToDrag = this;

	bIsBeingDragged = true;
	LocalCursorPosition = RootWidgetToDrag->GetCachedGeometry().AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());

	return FEventReply(true);
}

FEventReply UDragAndDropImage::OnMouseMove(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
	if (!bIsBeingDragged) return FEventReply(true);

	if (!RootWidgetToDrag) RootWidgetToDrag = this;

	FVector2D TranslationInViewport = 
		RootWidgetToDrag->GetCachedGeometry().AbsoluteToLocal(MouseEvent.GetScreenSpacePosition()) - 
		RootWidgetToDrag->GetCachedGeometry().AbsoluteToLocal(MouseEvent.GetLastScreenSpacePosition());

	RootWidgetToDrag->SetRenderTranslation(RootWidgetToDrag->RenderTransform.Translation + TranslationInViewport);

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