// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "Shaker.h"
#include "Customer.h"
#include "MyGameInstance.h"
#include "Glass.h"
#include "Trash.h"
#include "Ingredients.h"

void AMyPlayerController::BeginPlay()
{
    Super::BeginPlay();

    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;
    EnableInput(this);
    
    bInputBlocked = false;

    UE_LOG(LogTemp, Warning, TEXT("OUII"));
    FInputModeGameAndUI InputMode;
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    InputMode.SetHideCursorDuringCapture(false);
    SetInputMode(InputMode);

}

void AMyPlayerController::ForceRelease()
{
    SelectedActor = nullptr;
    isDragging = false;
    isDraggingShaker = false;
    bInputBlocked = true;
}

void AMyPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindAction("Pause", IE_Pressed, this, &AMyPlayerController::TogglePause);
    InputComponent->BindAction("Click", IE_Pressed, this, &AMyPlayerController::OnClickPressed);
    InputComponent->BindAction("Click", IE_Released, this, &AMyPlayerController::OnClickReleased);
}

void AMyPlayerController::TogglePause()
{
    if (pauseMenuInstance && pauseMenuInstance->IsInViewport())
    {
        UGameplayStatics::SetGamePaused(this, false);
        pauseMenuInstance->RemoveFromParent();
    }
    else
    {
        UGameplayStatics::SetGamePaused(this, true);
        pauseMenuInstance = CreateWidget<UUserWidget>(this, pauseMenuClass);
        pauseMenuInstance->AddToViewport();
    }
}

void AMyPlayerController::OnClickPressed()
{
    if (UGameplayStatics::IsGamePaused(this) || bInputBlocked)
        return;

    FHitResult Hit;
    GetHitResultUnderCursor(ECC_Visibility, false, Hit);
    
    AActor* HitActor = Hit.GetActor();
    if (!HitActor || !HitActor->IsValidLowLevel())
        return;

    if (!HitActor->IsA(ABlenderTop::StaticClass()))
        isDragging = true;
    if (HitActor->IsA(AShaker::StaticClass()))
        isDraggingShaker = true;
    if (HitActor->IsA(AIngredients::StaticClass()))
    {
        AIngredients* ingredient = Cast<AIngredients>(HitActor);
        if (ingredient && ingredient->getIngredientType() == EIngredientsTypes::gasoline)
            isDraggingGasoline = true;
    }

    SelectedActor = HitActor;
    initialLocation = SelectedActor->GetActorLocation();

    if (AIngredients* Ingredient = Cast<AIngredients>(SelectedActor))
        Ingredient->OnGrabbed();

    if (!SelectedActor || !SelectedActor->IsValidLowLevel())
        return;
    if (SelectedActor->IsA(ABlenderTop::StaticClass()))
    {
        ABlender* blender = Cast<ABlender>(UGameplayStatics::GetActorOfClass(GetWorld(), ABlender::StaticClass()));
        if (blender && blender->IsBlenderFusion())
            blender->isBlenderFusionFalse();
    }

    if (SelectedActor->IsA(ABlender::StaticClass()))
        return;

    FVector WorldLocation, WorldDirection;
    if (DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
        GrabOffset = SelectedActor->GetActorLocation() - WorldLocation;
}

bool AMyPlayerController::getIsDragging() const
{
    return isDragging;
}

bool AMyPlayerController::getIsDraggingShaker() const
{
    return isDraggingShaker;
}

bool AMyPlayerController::getIsDraggingGasoline() const
{
    return isDraggingGasoline;
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

    if (ATrash* trash = Cast<ATrash>(UGameplayStatics::GetActorOfClass(GetWorld(), ATrash::StaticClass())))
    {
        if (trash->getIsOverTrash())
        {
            trash->Throw();
        }
    }

    AGlass* glass = Cast<AGlass>(SelectedActor);
    if (glass) 
    {
        TArray<AActor*> allCustomers;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACustomer::StaticClass(), allCustomers);

        for (AActor* actor : allCustomers)
        {
            ACustomer* customer = Cast<ACustomer>(actor);
            if (customer && customer->getIsOverCustomer())
            {
                customer->SellDrink();
                break;
            }
        }
    }

    if (SelectedActor)
    {
        FVector Loc = SelectedActor->GetActorLocation();
        if (Loc.X < minX || Loc.X > maxX || Loc.Z < minZ || Loc.Z > maxZ)
            SelectedActor->SetActorLocation(initialLocation);
    }

    SelectedActor = nullptr;
    isDragging = false;
    isDraggingShaker = false;
    isDraggingGasoline = false;
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