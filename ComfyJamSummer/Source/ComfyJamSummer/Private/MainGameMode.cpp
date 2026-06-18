// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameInstance.h"
#include "MainGameMode.h"

void AMainGameMode::BeginPlay()
{
    Super::BeginPlay();
    UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
    if (GI)
        GI->PlayMusic(GI->gameMusic);

    BackgroundActorRef = Cast<ABackgroundActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ABackgroundActor::StaticClass()));
}

void AMainGameMode::OnDrinkSold()
{
    drinksSold++;

    if (BackgroundActorRef)
        BackgroundActorRef->UpdateBackground(drinksSold);

    if (drinksSold >= 10)
        AlienEnd();
}

void AMainGameMode::OnDrinkGasolineSold()
{
    drinksGasolineSold++;

    if (drinksGasolineSold >= 10)
        GoodEnd();
}

void AMainGameMode::GoodEnd()
{
    if (goodEndingScreen)
    {
        drinksGasolineSold = 0;
        drinksSold = 0;
        UUserWidget* EndScreen = CreateWidget<UUserWidget>(GetWorld(), goodEndingScreen);
        EndScreen->AddToViewport();
        UGameplayStatics::SetGamePaused(this, true);
        
        UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
        if (GI)
            GI->PlayMusic(GI->goodEndingMusic);
    }
}

void AMainGameMode::AlienEnd()
{
    if (alienEndingScreen)
    {
        drinksGasolineSold = 0;
        drinksSold = 0;
        UUserWidget* EndScreen = CreateWidget<UUserWidget>(GetWorld(), alienEndingScreen);
        EndScreen->AddToViewport();
        UGameplayStatics::SetGamePaused(this, true);
        
        UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
        if (GI)
            GI->PlayMusic(GI->badEndingMusic);
    }
}