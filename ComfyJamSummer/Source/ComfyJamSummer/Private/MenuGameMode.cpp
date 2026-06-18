// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuGameMode.h"
#include "MyGameInstance.h"
#include "MainMenu.h"

void AMenuGameMode::BeginPlay()
{
    Super::BeginPlay();


    UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
    if (GI)
        GI->PlayMusic(GI->menuMusic);

    if (!MenuClass)
    {
        return;
    }

    UMainMenu* Menu = CreateWidget<UMainMenu>(GetWorld(), MenuClass);

    if (Menu)
    {
        Menu->AddToViewport();
    }

    APlayerController* PC = GetWorld()->GetFirstPlayerController();

    if (PC)
    {
        PC->bShowMouseCursor = true;

        FInputModeUIOnly Mode;
        Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

        PC->SetInputMode(Mode);
    }

    menuWidget = CreateWidget<UUserWidget>(GetWorld(), MenuClass);
    menuWidget->AddToViewport();
}

void AMenuGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    if (menuWidget)
    {
        menuWidget->RemoveFromParent();
        menuWidget = nullptr;
    }
}