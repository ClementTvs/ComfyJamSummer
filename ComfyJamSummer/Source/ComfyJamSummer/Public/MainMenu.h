// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class COMFYJAMSUMMER_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
	
	public:

	virtual void NativeConstruct() override;

	protected:

		UPROPERTY(meta = (BindWidget))
		UButton* Button_Play;

		UPROPERTY(meta = (BindWidget))
		UButton* Button_Quit;

		UFUNCTION()
		void OnPlayClicked();

		UFUNCTION()
		void OnQuitClicked();
};
