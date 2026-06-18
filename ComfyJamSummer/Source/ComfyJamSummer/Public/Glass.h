// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MoveableSprite.h"
#include "Ingredients.h"
#include "BlenderTop.h"
#include "Drinks.h"
#include "Shaker.h"
#include "Components/ProgressBar.h"
#include "Components/WidgetComponent.h"
#include "Components/AudioComponent.h"
#include "Glass.generated.h"

class AShaker;
class ABlenderTop;

/**
 * 
 */
UCLASS()
class COMFYJAMSUMMER_API AGlass : public AMoveableSprite
{
	GENERATED_BODY()
	
	private:

	EDrinks drink = EDrinks::noDrink;
	FTimerHandle glassTimer;
	float timerDuration;
	bool isFill = false;

	UPROPERTY()
	ABlenderTop *pendingBlender = nullptr;
	UPROPERTY()
	AShaker *pendingShaker = nullptr;
	UPROPERTY()
	AIngredients *pendingIngredient = nullptr;

	UFUNCTION()
	void FillGlass();

	UFUNCTION()
	void OnBlenderOverlap(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	

	UFUNCTION()
	void OnBlenderEndOverlap(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	void StartPourSound();
	void StopPourSound();
	public:

	AGlass();

	void BeginPlay();
	virtual void Tick(float DeltaTime) override;

	EDrinks getDrink() const;
	
	UPROPERTY(VisibleAnywhere)
	UBoxComponent *fillHitBox;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	FVector spawnLocation;


	UPROPERTY(EditAnywhere, Category="Spawn")
	TSubclassOf<AGlass> glassClass;

	UPROPERTY(VisibleAnywhere)
	UPaperSpriteComponent* badDrinkSprite;

	UPROPERTY(VisibleAnywhere)
	UPaperSpriteComponent* pinaColadaSprite;

	UPROPERTY(VisibleAnywhere)
	UPaperSpriteComponent* daiquiriSprite;

	UPROPERTY(VisibleAnywhere)
	UPaperSpriteComponent* margaritaSprite;

	UPROPERTY(VisibleAnywhere)
	UPaperSpriteComponent* daiquiriSpriteGasoline;

	UPROPERTY(VisibleAnywhere)
	UPaperSpriteComponent* margaritaSpriteGasoline;
	
	UPROPERTY(VisibleAnywhere)
	UPaperSpriteComponent* pinaColadaSpriteGasoline;

	UPROPERTY(VisibleAnywhere)
	UPaperSpriteComponent* straightGasolineSprite;
	
	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* timerWidgetInstance;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> timerWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundBase* pourSound;

	UPROPERTY()
	UAudioComponent* pourAudio = nullptr;
};
