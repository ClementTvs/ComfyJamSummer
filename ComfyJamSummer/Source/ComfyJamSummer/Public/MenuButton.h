// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MenuButton.generated.h"

/**
 * 
 */
UCLASS()
class COMFYJAMSUMMER_API UMenuButton : public UUserWidget
{
	GENERATED_BODY()
	
	public:
    virtual void NativeConstruct() override;
    
    UPROPERTY(meta = (BindWidget))
    UButton* Button_Menu;
    
    UFUNCTION()
    void OnMenuClicked();
};
