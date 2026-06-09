// Fill out your copyright notice in the Description page of Project Settings.
#include "Alien.h"
#include "PaperFlipbookComponent.h"
#include "PaperZDAnimationComponent.h"
#include "TimerManager.h"

AAlien::AAlien()
{
    PrimaryActorTick.bCanEverTick = true;

    FlipbookComp = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipbookComp"));
    RootComponent = FlipbookComp;

    AnimComp = CreateDefaultSubobject<UPaperZDAnimationComponent>(TEXT("AnimComp"));
}

void AAlien::BeginPlay()
{
    Super::BeginPlay();

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, [this]()
	{
		State = (State == EAlienState::Idle) ? EAlienState::LookAway : EAlienState::Idle;
	}, 2.0f, true);
}

void AAlien::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AAlien::SetAlienState(EAlienState NewState) 
{
	State = NewState;
}