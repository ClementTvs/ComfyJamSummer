// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IngredientsTypes.h"
#include "Ingredients.h"
#include "BlenderTop.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ProgressBar.h"
#include "Components/WidgetComponent.h"
#include "Blender.generated.h"

/**
 * 
 */
UCLASS()
class COMFYJAMSUMMER_API ABlender : public AActor
{
	GENERATED_BODY()
	
	private:

	bool isOverBlender = false;
	bool isBlenderFusion = false;
	bool isBlenderWorking = false;

	FTimerHandle blenderTimer;

	// UFUNCTION()
	// void OnTopTouchBottom(UPrimitiveComponent* OverlappedComp,
	// 	AActor* OtherActor,
	// 	UPrimitiveComponent* OtherComp,
	// 	int32 OtherBodyIndex,
	// 	bool bFromSweep,
	// 	const FHitResult& SweepResult);

	UFUNCTION()
	void OnTopEnter(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnTopLeaveBottom(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	void BlenderStart();

	UFUNCTION()
	void OnBlenderClicked(UPrimitiveComponent* ClickedComp, FKey ButtonPressed);

	bool ContainsRecipe(const TArray<EIngredientsTypes>& Recipe);

	public:
	
	virtual void Tick(float DeltaTime) override;

	bool IsOverBlender() const;
	bool IsBlenderFusion() const;
	bool IsBlenderWorking() const;
	void isBlenderFusionFalse();
	float timerDuration;

	UFUNCTION()
	void FusionBlender();

	ABlender();
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	USceneComponent *root;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> timerWidgetClass;

	UPROPERTY()
	UWidgetComponent* timerWidgetInstance;

	UPROPERTY()
	ABlenderTop* blenderTopRef;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent *hitBox;

	UPROPERTY(VisibleAnywhere)
	UPaperSpriteComponent *blenderSprite;
};
