// SliderBar.h
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "SliderBar.generated.h"

UCLASS()
class COMFYJAMSUMMER_API USliderBar : public UUserWidget
{
    GENERATED_BODY()

	public:

		UPROPERTY(meta = (BindWidget))
		UImage* SliderBar;

		UPROPERTY(meta = (BindWidget))
		UImage* SliderHandle;

		UPROPERTY(BlueprintReadOnly)
		float sliderValue = 0.5f;

	protected:
		virtual void NativeConstruct() override;
		virtual FReply NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
		virtual FReply NativeOnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
		virtual FReply NativeOnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
		virtual void NativeOnMouseLeave(const FPointerEvent& MouseEvent) override;

	private:
		bool isDraggingSlider = false;
		void UpdateHandleFromValue();
};