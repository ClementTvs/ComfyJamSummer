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
		AMoveableSprite();

	protected:
		virtual void BeginPlay() override;

		UPROPERTY(VisibleAnywhere)
		UBoxComponent *hitBox;

		UPROPERTY(VisibleAnywhere)
		UPaperSpriteComponent* sprite;

		UPROPERTY(VisibleAnywhere)
		USceneComponent *root;
		
	public:	
	virtual void Tick(float DeltaTime) override;
	UPaperSpriteComponent* GetSprite() const;

};
