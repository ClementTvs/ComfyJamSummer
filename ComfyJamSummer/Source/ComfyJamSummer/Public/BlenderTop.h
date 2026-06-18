// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MoveableSprite.h"
#include "IngredientsTypes.h"
#include "Ingredients.h"
#include "Drinks.h"
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
	EDrinks drink = EDrinks::noDrink;
	bool isPouring = false;

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
	
	public:

	void StopPouring();
	void StartPouring();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void setDrink(EDrinks newDrink);
	EDrinks getDrink() const;
	void resetDrink();

	ABlenderTop();
	UPROPERTY(VisibleAnywhere)
	UBoxComponent *fillHitBox;

	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* timerWidgetInstance;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> timerWidgetClass;

	float timerDuration;

	const TArray<EIngredientsTypes>& getCurrentIngredients() const;
	void clearCurrentIngredients();
};
