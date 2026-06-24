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

class UPouring;

class AShaker;
class ABlenderTop;

UCLASS()
class COMFYJAMSUMMER_API AGlass : public AMoveableSprite
{
	GENERATED_BODY()

private:
	EDrinks Drink = EDrinks::noDrink;
	FTimerHandle glassTimer;
	float timerDuration;
	bool bIsFill = false;

	UPROPERTY()
	ABlenderTop *pendingBlender = nullptr;
	UPROPERTY()
	AShaker *pendingShaker = nullptr;
	UPROPERTY()
	AIngredients *pendingIngredient = nullptr;

	UPROPERTY()
	UPouring *pendingSpout = nullptr;

	UPROPERTY(EditAnywhere, Category = "Pour")
	float fillMaxDistance = 100.f;

	void UpdatePour();
	void TryAcquireSpout();
	void OnPourFinished();
	void CancelPour();
	void StartPourSound();
	void StopPourSound();

public:
	AGlass();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	EDrinks getDrink() const;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent *fillHitBox;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	FVector spawnLocation;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	TSubclassOf<AGlass> glassClass;

	UPROPERTY(EditAnywhere, Category = "Drinks")
	TMap<EDrinks, UPaperSprite *> DrinkSprites;

	UPROPERTY(VisibleAnywhere)
	UWidgetComponent *timerWidgetInstance;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> timerWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundBase *pourSound;

	UPROPERTY()
	UAudioComponent *pourAudio = nullptr;
};
