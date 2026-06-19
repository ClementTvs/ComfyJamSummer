// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Engine/HitResult.h"
#include "MoveableSprite.h"
#include "Blender.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class COMFYJAMSUMMER_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	

	protected:
		virtual void BeginPlay() override;
		virtual void SetupInputComponent() override;
		virtual void Tick(float DeltaTime) override;

	private:
		UPROPERTY()
		AActor* SelectedActor;
		FVector GrabOffset;

		FVector initialLocation;

		bool isDragging = false;
		bool isDraggingShaker = false;
		bool isDraggingGasoline = false;
		void OnClickPressed();
		void OnClickReleased();
		void TogglePause();
	    bool bInputBlocked = false;
	public:
		UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<UUserWidget> pauseMenuClass;
		UPROPERTY()
		UUserWidget* pauseMenuInstance;

		UPROPERTY(EditAnywhere, Category = "Bounds")
		float minX;
		UPROPERTY(EditAnywhere, Category = "Bounds")
		float maxX;
		UPROPERTY(EditAnywhere, Category = "Bounds")
		float minZ;
		UPROPERTY(EditAnywhere, Category = "Bounds")
		float maxZ;

		void ForceRelease();

		bool getIsDragging() const;
		bool getIsDraggingShaker() const;
		bool getIsDraggingGasoline() const;

};
