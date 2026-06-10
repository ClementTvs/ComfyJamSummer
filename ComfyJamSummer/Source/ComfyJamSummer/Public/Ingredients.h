// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MoveableSprite.h"
#include "IngredientsTypes.h"
#include "Ingredients.generated.h"

/**
 * 
 */
UCLASS()
class COMFYJAMSUMMER_API AIngredients : public AMoveableSprite
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	UPaperSpriteComponent *fillSprite;

	UPROPERTY(EditAnywhere)
	EIngredientsTypes	ingredientType;

	public:

	AIngredients();
	
	const EIngredientsTypes &getIngredientType() const;

    UFUNCTION(BlueprintCallable, Category = "Ingredient")
    void OnGrabbed();

    UFUNCTION(BlueprintCallable, Category = "Ingredient")
    void OnReleased();
};
