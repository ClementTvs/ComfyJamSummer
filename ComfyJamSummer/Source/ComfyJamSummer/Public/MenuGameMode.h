// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "MenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class COMFYJAMSUMMER_API AMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	public:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<class UMainMenu> MenuClass;
	
	UPROPERTY()
	UUserWidget* menuWidget;


	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
