// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "Shaker.h"
#include "Customer.h"
#include "Glass.h"
#include "Ingredients.h"

void AMyPlayerController::BeginPlay()
{
    Super::BeginPlay();

    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;
    EnableInput(this);

    UE_LOG(LogTemp, Warning, TEXT("OUII"));
    FInputModeGameAndUI InputMode;
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    InputMode.SetHideCursorDuringCapture(false);
    SetInputMode(InputMode);
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

    if (!Hit.GetActor())
        return;
    if (!Hit.GetActor()->IsA(ABlenderTop::StaticClass()))
        isDragging = true;
    if (!Hit.GetActor()->IsA(AShaker::StaticClass()))
        isDraggingShaker = true;
    if (Hit.GetActor())
    {
        SelectedActor = Hit.GetActor();
        initialLocation = SelectedActor->GetActorLocation();

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

bool AMyPlayerController::getIsDragging() const
{
    return isDragging;
}

bool AMyPlayerController::getIsDraggingShaker() const
{
    return isDraggingShaker;
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