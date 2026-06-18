// Fill out your copyright notice in the Description page of Project Settings.


#include "Kismet/GameplayStatics.h"
#include "MyGameInstance.h"
#include "PauseMenu.h"

void UPauseMenu::NativeConstruct()
{
    Super::NativeConstruct();
    if (Button_Resume)
        Button_Resume->OnClicked.AddDynamic(this, &UPauseMenu::OnResumeClicked);
    if (Button_Menu)
        Button_Menu->OnClicked.AddDynamic(this, &UPauseMenu::OnMenuClicked);
}

void UPauseMenu::OnResumeClicked()
{
    UGameplayStatics::SetGamePaused(this, false);
    RemoveFromParent();
}

void UPauseMenu::OnMenuClicked()
{
    UGameplayStatics::OpenLevel(this, "MainMenu");
}
