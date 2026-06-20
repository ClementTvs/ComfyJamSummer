#include "Glass.h"
#include "Drinks.h"
#include "MyPlayerController.h"
#include "Pouring.h"

AGlass::AGlass()
{
    PrimaryActorTick.bCanEverTick = true;

    fillHitBox = CreateDefaultSubobject<UBoxComponent>("FillHitBox");
    pinaColadaSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PinaColadaSprite"));
    daiquiriSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("DaiquiriSprite"));
    margaritaSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("margaritaSprite"));
    badDrinkSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BadDrinkSprite"));
    daiquiriSpriteGasoline = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("DaiquiriSpriteGasoline"));
    margaritaSpriteGasoline = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("MargaritaSpriteGasoline"));
    pinaColadaSpriteGasoline = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PinaColadaSpriteGasoline"));
    straightGasolineSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("StraightGasolineSprite"));
    timerWidgetInstance = CreateDefaultSubobject<UWidgetComponent>(TEXT("TimerWidget"));

    fillHitBox->SetupAttachment(root);
    badDrinkSprite->SetupAttachment(root);
    pinaColadaSprite->SetupAttachment(root);
    margaritaSprite->SetupAttachment(root);
    daiquiriSpriteGasoline->SetupAttachment(root);
    margaritaSpriteGasoline->SetupAttachment(root);
    pinaColadaSpriteGasoline->SetupAttachment(root);
    daiquiriSprite->SetupAttachment(root);
    straightGasolineSprite->SetupAttachment(root);
    timerWidgetInstance->SetupAttachment(root);
    timerWidgetInstance->SetWidgetSpace(EWidgetSpace::World);
    timerWidgetInstance->SetRelativeLocation(FVector(0.f, 0.f, 80.f));
    timerWidgetInstance->SetDrawSize(FVector2D(400.f, 80.f));
    timerWidgetInstance->SetRelativeScale3D(FVector(0.07f, 0.07f, 0.07f));
    timerWidgetInstance->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

    fillHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    fillHitBox->SetCollisionObjectType(ECC_WorldStatic);
    fillHitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    fillHitBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
    fillHitBox->SetGenerateOverlapEvents(true);

    timerDuration = 2.0f;
    isFill = false;
}

void AGlass::BeginPlay()
{
    Super::BeginPlay();
    timerWidgetInstance->SetWidgetClass(timerWidgetClass);

    hitBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
    glassSprites = {
        sprite,
        pinaColadaSprite, daiquiriSprite, margaritaSprite, badDrinkSprite,
        daiquiriSpriteGasoline, margaritaSpriteGasoline, pinaColadaSpriteGasoline,
        straightGasolineSprite
    };

    for (UPaperSpriteComponent* c : glassSprites)
    {
        if (!c) continue;
        c->SetCollisionObjectType(ECC_WorldDynamic);
        c->SetCollisionResponseToAllChannels(ECR_Ignore);
        c->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
        c->SetGenerateOverlapEvents(false);
        c->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
    sprite->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AGlass::FillGlass()
{
    StopPourSound();
    isFill = true;

	UPaperSpriteComponent* toShow = nullptr;
    switch (drink)
    {
        case EDrinks::pinaColada:
			toShow = pinaColadaSprite;
			break;
        case EDrinks::daiquiri:
			toShow = daiquiriSprite;
			break;
        case EDrinks::margarita:
			toShow = margaritaSprite;
			break;
        case EDrinks::pinaColadaG:
			toShow = pinaColadaSpriteGasoline;
			break;
        case EDrinks::daiquiriG:
			toShow = daiquiriSpriteGasoline;
			break;
        case EDrinks::margaritaG:
			toShow = margaritaSpriteGasoline;
			break;
        case EDrinks::badDrink:
			toShow = badDrinkSprite;
			break;
        case EDrinks::gasoline:
			toShow = straightGasolineSprite;
			break;
        default: 
			break;
    }
	ShowGlassSprite(toShow);
    
	if (pendingShaker)
    {
        AShaker* shaker = pendingShaker;
        pendingShaker = nullptr;

        shaker->resetDrink();
        if (shaker->shakerOpenSprite)
            shaker->shakerOpenSprite->SetVisibility(false);
        if (shaker->GetSprite())
            shaker->GetSprite()->SetVisibility(true);
    }
    else if (pendingBlender)
    {
        ABlenderTop* blender = pendingBlender;
        pendingBlender = nullptr;
        blender->resetDrink();
    }

    pendingIngredient = nullptr;
    pendingSpout = nullptr;
}

void AGlass::CancelPour()
{
	UE_LOG(LogTemp, Warning, TEXT("CANCEL POUR"));
    GetWorld()->GetTimerManager().ClearTimer(glassTimer);
    StopPourSound();
    pendingBlender = nullptr;
    pendingShaker = nullptr;
    pendingIngredient = nullptr;
    pendingSpout = nullptr;
}

EDrinks AGlass::getDrink() const 
{ 
	return drink;
}

void AGlass::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UpdatePour();

    if (GetWorld()->GetTimerManager().IsTimerActive(glassTimer) && timerWidgetInstance)
    {
        UUserWidget* Widget = timerWidgetInstance->GetUserWidgetObject();
        if (Widget)
        {
            Widget->SetVisibility(ESlateVisibility::Visible);
            float Progress = GetWorld()->GetTimerManager().GetTimerRemaining(glassTimer) / timerDuration;
            if (UProgressBar* Bar = Cast<UProgressBar>(Widget->GetWidgetFromName(TEXT("TimerProgressBar"))))
                Bar->SetPercent(Progress);
        }
    }
    else if (timerWidgetInstance)
    {
        UUserWidget* Widget = timerWidgetInstance->GetUserWidgetObject();
        if (Widget)
            Widget->SetVisibility(ESlateVisibility::Hidden);
    }
}

void AGlass::StartPourSound()
{
    if (pourSound && !pourAudio)
        if (UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance()))
            pourAudio = GI->SpawnSFX(pourSound, 3.f);
}

void AGlass::StopPourSound()
{
    if (pourAudio)
    {
        pourAudio->Stop();
        pourAudio = nullptr;
    }
}

void AGlass::UpdatePour()
{
    if (isFill)
        return;

    AMyPlayerController *pc = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());

    if (pendingSpout)
    {
        AActor* owner = pendingSpout->GetOwner();
        if (!IsValid(owner))
        {
            CancelPour();
            return;
        }

        const FVector d = owner->GetActorLocation() - GetActorLocation();
        const float dist = FVector(d.X, 0.f, d.Z).Size();

        if (dist <= fillMaxDistance)
        {
            const float side = (owner->GetActorLocation().X > GetActorLocation().X) ? 1.f : -1.f;
			UE_LOG(LogTemp, Warning, TEXT("KEEP dist=%f pendingSpout=%d"), dist, pendingSpout ? 1 : 0);
            pendingSpout->KeepPouring(side);
        }
        else
        {
            CancelPour();
        }
    }
    else
    {
        TryAcquireSpout();
    }
}

void AGlass::TryAcquireSpout()
{
    AMyPlayerController* pc = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
    if (!pc)
        return;

    TArray<AActor*> over;
    fillHitBox->GetOverlappingActors(over);

    for (AActor* a : over)
    {
        if (a->IsA(ABlenderTop::StaticClass()) && !pc->getIsDragging())
        {
            ABlenderTop* bt = Cast<ABlenderTop>(a);
            if (!bt || bt->getDrink() == EDrinks::noDrink) continue;
            pendingBlender = bt;
            drink = bt->getDrink();
        }
        else if (a->IsA(AShaker::StaticClass()) && pc->getIsDraggingShaker())
        {
            AShaker* sh = Cast<AShaker>(a);
            if (!sh || sh->getDrink() == EDrinks::noDrink) continue;
            pendingShaker = sh;
            drink = sh->getDrink();
        }
        else if (a->IsA(AIngredients::StaticClass()) && pc->getIsDraggingGasoline())
        {
            AIngredients* ing = Cast<AIngredients>(a);
            if (!ing || ing->getIngredientType() != EIngredientsTypes::gasoline) continue;
            pendingIngredient = ing;
            drink = EDrinks::gasoline;
        }
        else
        {
            continue;
        }

        pendingSpout  = a->FindComponentByClass<UPouring>();
        GetWorld()->GetTimerManager().SetTimer(glassTimer, this, &AGlass::FillGlass, timerDuration, false);
        StartPourSound();
        return;
    }
}

void AGlass::ShowGlassSprite(UPaperSpriteComponent* toShow)
{
    if (!toShow)
        toShow = sprite;

    for (UPaperSpriteComponent* c : glassSprites)
    {
        if (!c) continue;
        c->SetVisibility(false);
        c->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

    toShow->SetVisibility(true);
    toShow->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}