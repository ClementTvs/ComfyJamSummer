// Fill out your copyright notice in the Description page of Project Settings.

#include "Ingredients.h"
#include "Kismet/GameplayStatics.h"
#include "Shaker.h"
#include "Glass.h"
#include "BlenderTop.h"
#include "Alien.h"


void AIngredients::BeginPlay()
{
    Super::BeginPlay();

    hitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    hitBox->SetCollisionObjectType(ECC_WorldDynamic);
    hitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    hitBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
    hitBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
    hitBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	hitBox->SetUsingAbsoluteRotation(true); 
    hitBox->SetGenerateOverlapEvents(true);

    UE_LOG(LogTemp, Warning, TEXT("INGREDIENT %s: WorldStatic resp=%d, enabled=%d, genOverlap=%d"),
        *GetName(),
        (int)hitBox->GetCollisionResponseToChannel(ECC_WorldStatic),
        (int)hitBox->GetCollisionEnabled(),
        hitBox->GetGenerateOverlapEvents() ? 1 : 0);
}

AIngredients::AIngredients()
{
    PrimaryActorTick.bCanEverTick = true;

    fillSprite = CreateDefaultSubobject<UPaperSpriteComponent>("FillSprite");
    fillSprite->SetupAttachment(root);
    fillSprite->SetHiddenInGame(true);

    hitBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    hitBox->SetCollisionObjectType(ECC_WorldDynamic);
    hitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    hitBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
    hitBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	hitBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
    hitBox->SetGenerateOverlapEvents(true);
}
	
const EIngredientsTypes &AIngredients::getIngredientType() const
{
    return ingredientType;
}


void AIngredients::OnGrabbed()
{
    if (ingredientType == EIngredientsTypes::gasoline)
    {
        if (AAlien* Alien = Cast<AAlien>(
            UGameplayStatics::GetActorOfClass(GetWorld(), AAlien::StaticClass())))
        {
            Alien->OnGasolineGrab();
            Alien->bGasolineInUse = true;
        }
    }
}

void AIngredients::OnReleased()
{
    if (ingredientType == EIngredientsTypes::gasoline)
    {
        if (AAlien* Alien = Cast<AAlien>(
            UGameplayStatics::GetActorOfClass(GetWorld(), AAlien::StaticClass())))
        {
            Alien->bGasolineInUse = false;
        }
    }
}

void AIngredients::StartPouring(bool bShouldTiltLeft)
{
    isPouring = true;
    this->bTiltLeft = bShouldTiltLeft;
}

void AIngredients::StopPouring()
{
    isPouring = false;
    SetActorRotation(FRotator(0.f, 0.f, 0.f));
}

void AIngredients::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if (isPouring)
    {
        TArray<AActor*> overlappingActors;
        GetOverlappingActors(overlappingActors);
        
        bool stillOverlapping = false;
        for (AActor* actor : overlappingActors)
        {
            if (actor->IsA(ABlenderTop::StaticClass()) || actor->IsA(AShaker::StaticClass()) || actor->IsA(AGlass::StaticClass()))
            {
                stillOverlapping = true;
                break;
            }
        }
        if (!stillOverlapping)
        {
            StopPouring();
            return;
        }

        FRotator CurrentRotation = GetActorRotation();
        float TargetPitch = 0.0f;
        if (ingredientType == EIngredientsTypes::gasoline)
        {
            TargetPitch = 70.f;
        }    
        else
        {
            TargetPitch = -70.f;
            if (bTiltLeft)
                TargetPitch = 70.f;
        }
        CurrentRotation.Pitch = FMath::FInterpTo(CurrentRotation.Pitch, TargetPitch, DeltaTime, 3.f);
        SetActorRotation(CurrentRotation);
    }
}