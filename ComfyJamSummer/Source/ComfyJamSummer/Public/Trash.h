// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MoveableSprite.h"
#include "Glass.h"
#include "Trash.generated.h"


UCLASS()
class COMFYJAMSUMMER_API ATrash : public AMoveableSprite
{
	GENERATED_BODY()
	
	private :

		bool isOverTrash = false;

		UFUNCTION()
		void OnGlassOverlap(UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

		UFUNCTION()
		void OnGlassLeave(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	public:

		ATrash();
	
		bool getIsOverTrash() const;
		void Throw();

		UPROPERTY(EditAnywhere, Category = "Glass")
		FVector spawnLocation = FVector::ZeroVector;

		UPROPERTY(EditAnywhere, Category = "Glass")
		TSubclassOf<AGlass> glassClass;
};
