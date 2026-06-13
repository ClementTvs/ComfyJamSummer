// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperSpriteComponent.h"
#include "PlayerHealth.generated.h"

UCLASS()
class COMFYJAMSUMMER_API APlayerHealth : public AActor
{
	GENERATED_BODY()
	
	private:	
		int lives = 3;
		
	public:	
		APlayerHealth();
		void LoseLife();

		UPROPERTY(VisibleAnywhere)
		TArray<UPaperSpriteComponent*> heartsFullArray;
		UPROPERTY(VisibleAnywhere)
		TArray<UPaperSpriteComponent*> heartsEmptyArray;

		UPROPERTY(VisibleAnywhere)
		USceneComponent *root;

};
