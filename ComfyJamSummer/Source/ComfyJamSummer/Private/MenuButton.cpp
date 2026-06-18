// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "MyGameInstance.h"
#include "MenuButton.h"

void UMenuButton::NativeConstruct()
{
    Super::NativeConstruct();
    if (Button_Menu)
        Button_Menu->OnClicked.AddDynamic(this, &UMenuButton::OnMenuClicked);
}

void UMenuButton::OnMenuClicked()
{
    UGameplayStatics::OpenLevel(this, "MainMenu");
}