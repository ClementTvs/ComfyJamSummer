// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "Alien.h"
#include "Ingredients.h"


AIngredients::AIngredients()
{
    fillSprite = CreateDefaultSubobject<UPaperSpriteComponent>("FillSprite");
    fillSprite->SetupAttachment(root);
    fillSprite->SetHiddenInGame(true);

    hitBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    hitBox->SetCollisionObjectType(ECC_WorldDynamic);
    hitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    hitBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
    hitBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
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