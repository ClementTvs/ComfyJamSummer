// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "Ingredients.h"

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
    {
        SelectedActor = Hit.GetActor();

        if (AIngredients* Ingredient = Cast<AIngredients>(SelectedActor))
        {
            Ingredient->OnGrabbed();
        }
        if (SelectedActor->IsA(ABlenderTop::StaticClass()))
        {
            ABlender* blender = Cast<ABlender>(UGameplayStatics::GetActorOfClass(GetWorld(), ABlender::StaticClass()));
            if (blender->IsBlenderFusion())
            {
                blender->isBlenderFusionFalse();
            }
        }

		if (SelectedActor->IsA(ABlender::StaticClass()))
                return ;
        FVector WorldLocation, WorldDirection;
        if (DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
            GrabOffset = SelectedActor->GetActorLocation() - WorldLocation;
    }
}

void AMyPlayerController::OnClickReleased()
{
	if (AIngredients* Ingredient = Cast<AIngredients>(SelectedActor))
		Ingredient->OnReleased();
    if (ABlender* blender = Cast<ABlender>(UGameplayStatics::GetActorOfClass(GetWorld(), ABlender::StaticClass())))
    {
        if (blender->IsOverBlender())
        {
            blender->FusionBlender();
        }
    }
    SelectedActor = nullptr;
}

void AMyPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (!SelectedActor)
        return;

    FVector WorldLocation;
    FVector WorldDirection;
    if (DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
    {
        FVector NewLocation = SelectedActor->GetActorLocation();
        NewLocation.X = WorldLocation.X + GrabOffset.X;
        NewLocation.Z = WorldLocation.Z + GrabOffset.Z;
        SelectedActor->SetActorLocation(NewLocation);
    }
}