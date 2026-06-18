// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "PauseMenu.generated.h"

/**
 * 
 */
UCLASS()
class COMFYJAMSUMMER_API UPauseMenu : public UUserWidget
{
    GENERATED_BODY()
    
    public:
    virtual void NativeConstruct() override;
    
    UPROPERTY(meta = (BindWidget))
    UButton* Button_Resume;
    
    UPROPERTY(meta = (BindWidget))
    UButton* Button_Menu;
    
    UFUNCTION()
    void OnResumeClicked();
    
    UFUNCTION()
    void OnMenuClicked();
};