// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IngredientsTypes.h"
#include "Ingredients.h"
#include "BlenderTop.h"
#include "Kismet/GameplayStatics.h"
#include "Blender.generated.h"

/**
 * 
 */
UCLASS()
class COMFYJAMSUMMER_API ABlender : public AActor
{
	GENERATED_BODY()
	
	private:


	UFUNCTION()
	void OnTopTouchBottom(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnBlenderClicked(UPrimitiveComponent* ClickedComp, FKey ButtonPressed);

	bool ContainsRecipe(const TArray<EIngredientsTypes>& Recipe);

	public:

	ABlender();
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	USceneComponent *root;

	UPROPERTY()
	ABlenderTop* blenderTopRef;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent *hitBox;

	UPROPERTY(VisibleAnywhere)
	UPaperSpriteComponent *completeBlenderSprite;

	UPROPERTY(VisibleAnywhere)
	UPaperSpriteComponent *blenderSprite;
};
