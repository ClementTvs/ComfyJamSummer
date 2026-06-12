// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MoveableSprite.h"
#include "Ingredients.h"
#include "Glass.generated.h"

/**
 * 
 */
UCLASS()
class COMFYJAMSUMMER_API AGlass : public AMoveableSprite
{
	GENERATED_BODY()
	
	private:

	// UFUNCTION()
	// void OnIngredientOverlap(UPrimitiveComponent* OverlappedComp,
	// 	AActor* OtherActor,
	// 	UPrimitiveComponent* OtherComp,
	// 	int32 OtherBodyIndex,
	// 	bool bFromSweep,
	// 	const FHitResult& SweepResult);

	public:

	AGlass();
	
	UPROPERTY(VisibleAnywhere)
	UBoxComponent *fillHitBox;

};