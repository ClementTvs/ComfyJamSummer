// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MoveableSprite.h"
#include "IngredientsTypes.h"
#include "Ingredients.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/WidgetComponent.h"
#include "BlenderTop.generated.h"

/**
 * 
 */
UCLASS()
class COMFYJAMSUMMER_API ABlenderTop : public AMoveableSprite
{
	GENERATED_BODY()
	
	private:

	UPROPERTY()
	TArray<EIngredientsTypes> currentIngredients;
	AIngredients *pendingIngredient = nullptr;
	FTimerHandle IngredientTimer;

	UFUNCTION()
	void OnIngredientOverlap(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnIngredientEndOverlap(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	UFUNCTION()
	void ValidateIngredient();

	public :

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	ABlenderTop();
	UPROPERTY(VisibleAnywhere)
	UBoxComponent *fillHitBox;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> timerWidgetClass;

	float timerDuration;

	UPROPERTY()
	UWidgetComponent* timerWidgetInstance;

	const TArray<EIngredientsTypes>& getCurrentIngredients() const;
	void clearCurrentIngredients();
};
