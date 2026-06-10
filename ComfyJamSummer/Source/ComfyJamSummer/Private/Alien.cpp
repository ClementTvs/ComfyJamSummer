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
	StartWatching();
}

void AAlien::StartWatching()
{
	State = EAlienState::Idle;

	if (bGasolineInUse)
		OnPlayerCaught();

	float Duration = FMath::RandRange(WatchTimeMin, WatchTimeMax);
	GetWorldTimerManager().SetTimer(StateTimer, this, &AAlien::StartLookingAway, Duration, false);
}

void AAlien::StartLookingAway()
{
	State = EAlienState::LookAway;
	float Duration = FMath::RandRange(LookAwayTimeMin, LookAwayTimeMax);
	GetWorldTimerManager().SetTimer(StateTimer, this, &AAlien::StartWatching, Duration, false);
}

void AAlien::OnGasolineGrab()
{
	if (isWatching())
	{
		OnPlayerCaught();
	}
}

void AAlien::OnPlayerCaught()
{
	State = EAlienState::Idle;
	UE_LOG(LogTemp, Warning, TEXT("Player caught"));
}

void AAlien::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AAlien::SetAlienState(EAlienState NewState) 
{
	State = NewState;
}

bool AAlien::isWatching() const
{
	return State == EAlienState::Idle;
}