// Fill out your copyright notice in the Description page of Project Settings.

#include "Ingredients.h"


AIngredients::AIngredients()
{
    fillSprite = CreateDefaultSubobject<UPaperSpriteComponent>("FillSprite");
    fillSprite->SetupAttachment(moveable);
    fillSprite->SetHiddenInGame(true);

    hitBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    hitBox->SetCollisionObjectType(ECC_WorldDynamic);
    hitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    hitBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block); // souris
    hitBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap); // overlap avec blender
    hitBox->SetGenerateOverlapEvents(true);
}
	
const EIngredientsTypes &AIngredients::getIngredientType() const
{
    return ingredientType;
}