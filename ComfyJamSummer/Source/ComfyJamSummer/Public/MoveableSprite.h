// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "PaperSpriteComponent.h"
#include "MoveableSprite.generated.h"

UCLASS()
class COMFYJAMSUMMER_API AMoveableSprite : public AActor
{
	GENERATED_BODY()
	
	public:	
	// Sets default values for this actor's properties
		AMoveableSprite();

	protected:
	// Called when the game starts or when spawned
		virtual void BeginPlay() override;

		UPROPERTY(VisibleAnywhere)
		UBoxComponent *hitBox;

		UPROPERTY(VisibleAnywhere)
		UPaperSpriteComponent* sprite;

		UPROPERTY(VisibleAnywhere)
		USceneComponent *root;

		UPROPERTY(VisibleAnywhere)
		USceneComponent *moveable;
		
	public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    USceneComponent* GetMoveable() const;

};
