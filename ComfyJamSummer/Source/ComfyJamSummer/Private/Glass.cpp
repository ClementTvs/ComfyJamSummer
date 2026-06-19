#include "Drinks.h"
#include "MyPlayerController.h"
#include "Glass.h"
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
    fillHitBox->OnComponentBeginOverlap.AddDynamic(this, &AGlass::OnBlenderOverlap);
    fillHitBox->OnComponentEndOverlap.AddDynamic(this, &AGlass::OnBlenderEndOverlap);

    timerDuration = 2.0f;
    isFill = false;
}

void AGlass::BeginPlay()
{
    Super::BeginPlay();
    timerWidgetInstance->SetWidgetClass(timerWidgetClass);
}

void AGlass::FillGlass()
{
    StopPourSound();
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
        case EDrinks::pinaColadaG:
			pinaColadaSpriteGasoline->SetVisibility(true);
			break;
        case EDrinks::daiquiriG:
			daiquiriSpriteGasoline->SetVisibility(true);
			break;
        case EDrinks::margaritaG:
			margaritaSpriteGasoline->SetVisibility(true);
			break;
        case EDrinks::badDrink:
			badDrinkSprite->SetVisibility(true);
			break;
        case EDrinks::gasoline:
			straightGasolineSprite->SetVisibility(true);
			break;
        default: 
			break;
    }

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

void AGlass::OnBlenderOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetWorld()->GetTimerManager().IsTimerActive(glassTimer))
        return;
    AMyPlayerController *pc = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());

    if (OtherActor && OtherActor->IsA(ABlenderTop::StaticClass()) && isFill == false && !pc->getIsDragging())
    {
        pendingBlender = Cast<ABlenderTop>(OtherActor);
        drink = pendingBlender->getDrink();
        if (drink == EDrinks::noDrink) 
		{ 
			pendingBlender = nullptr; 
			return; 
		}

        pendingSpout = OtherActor->FindComponentByClass<UPouring>();
        GetWorld()->GetTimerManager().SetTimer(glassTimer, this, &AGlass::FillGlass, timerDuration, false);
        StartPourSound();
    }
    else if (OtherActor && OtherActor->IsA(AShaker::StaticClass()) && isFill == false && pc->getIsDraggingShaker())
    {
        pendingShaker = Cast<AShaker>(OtherActor);
        drink = pendingShaker->getDrink();
        if (drink == EDrinks::noDrink) 
		{ 
			pendingShaker = nullptr; 
			return; 
		}

        pendingSpout = OtherActor->FindComponentByClass<UPouring>();
        GetWorld()->GetTimerManager().SetTimer(glassTimer, this, &AGlass::FillGlass, timerDuration, false);
        StartPourSound();
    }
    else if (OtherActor && OtherActor->IsA(AIngredients::StaticClass()) && isFill == false)
    {
        AIngredients* ingredient = Cast<AIngredients>(OtherActor);
        if (ingredient && ingredient->getIngredientType() == EIngredientsTypes::gasoline)
        {
            pendingIngredient = ingredient;
            drink = EDrinks::gasoline;
            pendingSpout = OtherActor->FindComponentByClass<UPouring>();
            GetWorld()->GetTimerManager().SetTimer(glassTimer, this, &AGlass::FillGlass, timerDuration, false);
            StartPourSound();
        }
    }
}

void AGlass::OnBlenderEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (pendingSpout && OtherActor == pendingSpout->GetOwner())
        CancelPour();
}

void AGlass::CancelPour()
{
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

    if (pendingSpout)
    {
        AActor* spoutOwner = pendingSpout->GetOwner();
        TArray<AActor*> over;
        fillHitBox->GetOverlappingActors(over);

        if (IsValid(spoutOwner) && over.Contains(spoutOwner))
        {
            const float side = (spoutOwner->GetActorLocation().X > GetActorLocation().X) ? 1.f : -1.f;
            pendingSpout->KeepPouring(side);
        }
        else
        {
            CancelPour();
        }
    }

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
        pourAudio = UGameplayStatics::SpawnSound2D(this, pourSound);
}

void AGlass::StopPourSound()
{
    if (pourAudio)
    {
        pourAudio->Stop();
        pourAudio = nullptr;
    }
}