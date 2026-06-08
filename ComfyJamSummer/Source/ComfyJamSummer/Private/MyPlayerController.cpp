// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

void AMyPlayerController::BeginPlay()
{
    Super::BeginPlay();

    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;
    EnableInput(this);
}

void AMyPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    InputComponent->BindAction("Click", IE_Pressed, this, 
        &AMyPlayerController::OnClickPressed);
    InputComponent->BindAction("Click", IE_Released, this, 
        &AMyPlayerController::OnClickReleased);
}

void AMyPlayerController::OnClickPressed()
{
    FHitResult Hit;
    GetHitResultUnderCursor(ECC_Visibility, false, Hit);
    if (Hit.GetActor())
        SelectedActor = Hit.GetActor();
}

void AMyPlayerController::OnClickReleased()
{
    SelectedActor = nullptr;
}

void AMyPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (SelectedActor)
    {
        FHitResult Hit;
        GetHitResultUnderCursor(ECC_Visibility, false, Hit);
        SelectedActor->SetActorLocation(Hit.Location);
    }
}