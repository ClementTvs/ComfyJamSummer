// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "BackgroundActor.h"
#include "MainGameMode.generated.h"

class UBackgroundActor;

UCLASS()
class COMFYJAMSUMMER_API AMainGameMode : public AGameModeBase
{
    GENERATED_BODY()

	public:
		virtual void BeginPlay() override;

		void OnDrinkSold();
		void OnDrinkGasolineSold();

		UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<UUserWidget> alienEndingScreen;

		UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<UUserWidget> goodEndingScreen;

	private:

		UPROPERTY()
		ABackgroundActor* BackgroundActorRef;

		int8 drinksSold = 0;
		int8 drinksGasolineSold = 0;

		void AlienEnd();
		void GoodEnd();

};
