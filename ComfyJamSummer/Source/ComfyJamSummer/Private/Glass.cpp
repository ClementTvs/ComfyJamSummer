// Fill out your copyright notice in the Description page of Project Settings.

#include "Drinks.h"
#include "MyPlayerController.h"
#include "Glass.h"

AGlass::AGlass()
{
    PrimaryActorTick.bCanEverTick = true;

    fillHitBox = CreateDefaultSubobject<UBoxComponent>("FillHitBox");
    pinaColadaSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PinaColadaSprite"));
	daiquiriSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("DaiquiriSprite"));
	margaritaSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("margaritaSprite"));
    badDrinkSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BadDrinkSprite"));
    timerWidgetInstance = CreateDefaultSubobject<UWidgetComponent>(TEXT("TimerWidget"));

    fillHitBox->SetupAttachment(root);
    badDrinkSprite->SetupAttachment(root);
    pinaColadaSprite->SetupAttachment(root);
    margaritaSprite->SetupAttachment(root);
    daiquiriSprite->SetupAttachment(root);
    timerWidgetInstance->SetupAttachment(root);

    fillHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    fillHitBox->SetCollisionObjectType(ECC_WorldStatic);
    fillHitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    fillHitBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
    fillHitBox->SetGenerateOverlapEvents(true);
    fillHitBox->OnComponentBeginOverlap.AddDynamic(this, &AGlass::OnBlenderOverlap);
    fillHitBox->OnComponentEndOverlap.AddDynamic(this, &AGlass::OnBlenderEndOverlap);

    timerDuration = 2.0f;
    isFill = false;
}

void AGlass::BeginPlay()
{
    Super::BeginPlay();

    timerWidgetInstance->SetWidgetSpace(EWidgetSpace::World);
    timerWidgetInstance->SetRelativeLocation(FVector(0.f, 0.f, 40.f));
    timerWidgetInstance->SetDrawSize(FVector2D(400.f, 80.f));
    timerWidgetInstance->SetWorldScale3D(FVector(0.07f, 0.07f, 0.07f));
    timerWidgetInstance->SetWorldRotation(FRotator(0.f, -90.f, 0.f));

    timerWidgetInstance->SetWidgetClass(timerWidgetClass);
    pinaColadaSprite->SetVisibility(false);
    badDrinkSprite->SetVisibility(false);
}

void AGlass::FillGlass()
{
    isFill = true;
    sprite->SetVisibility(false);

    switch (drink)
    {
		case EDrinks::pinaColada:
			pinaColadaSprite->SetVisibility(true);
			break;
		case EDrinks::daiquiri:
			daiquiriSprite->SetVisibility(true);
			break;
		case EDrinks::margarita:
			margaritaSprite->SetVisibility(true);
			break;
		case EDrinks::badDrink:
			badDrinkSprite->SetVisibility(true);
			break;
		default: 
			break;
    }
	
    if (pendingBlender)
        pendingBlender->setDrink(EDrinks::noDrink);
    if (drink == EDrinks::badDrink)
        badDrinkSprite->SetVisibility(true);
    if (drink == EDrinks::pinaColada)
        pinaColadaSprite->SetVisibility(true);
    if (pendingShaker)
    {
        pendingShaker->resetDrink();
        pendingShaker->shakerOpenSprite->SetVisibility(false);
        pendingShaker->GetSprite()->SetVisibility(true); 
    }
    else
        pendingBlender->resetDrink();
}

void AGlass::OnBlenderOverlap(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
    AMyPlayerController *pc = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());

    UE_LOG(LogTemp, Warning, TEXT("MOAUIS"));


    UE_LOG(LogTemp, Warning, TEXT("OUI"));
    if (OtherActor && OtherActor->IsA(ABlenderTop::StaticClass()) && isFill == false && !pc->getIsDragging())
    {
        pendingBlender = Cast<ABlenderTop>(OtherActor);
        drink = pendingBlender->getDrink();

        UE_LOG(LogTemp, Warning, TEXT("IN"));

        if (pendingBlender->getDrink() == EDrinks::noDrink)
            return ;
        UE_LOG(LogTemp, Warning, TEXT("SIU"));
        pendingBlender->StartPouring();
        if (drink != EDrinks::noDrink)
            GetWorld()->GetTimerManager().SetTimer(glassTimer, this, &AGlass::FillGlass, timerDuration, false);
    }
    else if (OtherActor && OtherActor->IsA(AShaker::StaticClass()) && isFill == false && pc->getIsDraggingShaker())
    {
        pendingShaker = Cast<AShaker>(OtherActor);
        drink = pendingShaker->getDrink();

        if (pendingShaker->getDrink() == EDrinks::noDrink)
            return ;
        bool bTiltLeft = pendingShaker->GetActorLocation().X > GetActorLocation().X;
        pendingShaker->StartPouring(bTiltLeft);
        if (drink != EDrinks::noDrink)
            GetWorld()->GetTimerManager().SetTimer(glassTimer, this, &AGlass::FillGlass, timerDuration, false);
    }

}

void AGlass::OnBlenderEndOverlap(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
    if (OtherActor == pendingBlender)
    {
        pendingBlender->StopPouring();
        GetWorld()->GetTimerManager().ClearTimer(glassTimer);
        pendingBlender = nullptr;
    }
    else if (OtherActor == pendingShaker)
    {
        pendingShaker->StopPouring();
        GetWorld()->GetTimerManager().ClearTimer(glassTimer);
        pendingShaker  = nullptr;
    }
}

EDrinks AGlass::getDrink() const
{
    return drink;
}


void AGlass::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (GetWorld()->GetTimerManager().IsTimerActive(glassTimer) && timerWidgetInstance)
    {

        UUserWidget* Widget = timerWidgetInstance->GetUserWidgetObject();

        if (Widget)
        {
            Widget->SetVisibility(ESlateVisibility::Visible);

            float Remaining = GetWorld()->GetTimerManager().GetTimerRemaining(glassTimer);
            float Progress = Remaining / timerDuration;

            UProgressBar* Bar = Cast<UProgressBar>(
                Widget->GetWidgetFromName(TEXT("TimerProgressBar"))
            );

            if (Bar)
            {
                Bar->SetPercent(Progress);
            }
        }
    }
    else if (timerWidgetInstance)
    {
        UUserWidget* Widget = timerWidgetInstance->GetUserWidgetObject();

        if (Widget)
        {
            Widget->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}
