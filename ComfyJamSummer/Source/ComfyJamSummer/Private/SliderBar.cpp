// SliderBar.cpp
#include "SliderBar.h"

void USliderBar::NativeConstruct()
{
    Super::NativeConstruct();
    FTimerHandle InitTimer;
    GetWorld()->GetTimerManager().SetTimer(InitTimer, [this]()
    {
        UpdateHandleFromValue();
    }, 0.1f, false);

}

FReply USliderBar::NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    isDraggingSlider = true;
    if (SliderBar)
    {
        FVector2D LocalPos = SliderBar->GetCachedGeometry().AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
        FVector2D BarSize = SliderBar->GetCachedGeometry().GetLocalSize();
        sliderValue = FMath::Clamp(LocalPos.X / BarSize.X, 0.f, 1.f);
        UpdateHandleFromValue();
    }
    return FReply::Handled();
}

FReply USliderBar::NativeOnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    if (!isDraggingSlider)
        return FReply::Unhandled();
    if (SliderBar)
    {
        FVector2D LocalPos = SliderBar->GetCachedGeometry().AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
        FVector2D BarSize = SliderBar->GetCachedGeometry().GetLocalSize();
        sliderValue = FMath::Clamp(LocalPos.X / BarSize.X, 0.f, 1.f);
        UpdateHandleFromValue();
    }
    return FReply::Handled();
}

FReply USliderBar::NativeOnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    isDraggingSlider = false;
    return FReply::Handled();
}

void USliderBar::NativeOnMouseLeave(const FPointerEvent& MouseEvent)
{
    isDraggingSlider = false;
}

void USliderBar::UpdateHandleFromValue()
{
    if (!SliderHandle || !SliderBar)
        return;
    UCanvasPanelSlot* HandleSlot = Cast<UCanvasPanelSlot>(SliderHandle->Slot);
    if (HandleSlot)
    {
        FVector2D BarSize = SliderBar->GetCachedGeometry().GetLocalSize();
        FVector2D HandleSize = HandleSlot->GetSize();
        float NewX = FMath::Clamp(sliderValue * BarSize.X - HandleSize.X / 2.f, 0.f, BarSize.X - HandleSize.X);
        HandleSlot->SetPosition(FVector2D(NewX, HandleSlot->GetPosition().Y));
    }
    UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
    if (GI)
        GI->SetVolume(sliderValue);
}