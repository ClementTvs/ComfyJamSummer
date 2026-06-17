// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MusicTrigger.generated.h"

UCLASS()
class COMFYJAMSUMMER_API AMusicTrigger : public AActor
{
	GENERATED_BODY()
	
	private:	
		UPROPERTY(EditAnywhere)
		USoundBase* gameMusic;

	protected:
		virtual void BeginPlay() override;

};
