#include "Glass.h"
#include "Drinks.h"
#include "MyPlayerController.h"
#include "Pouring.h"

AGlass::AGlass()
{
	PrimaryActorTick.bCanEverTick = true;

	fillHitBox = CreateDefaultSubobject<UBoxComponent>("FillHitBox");
	timerWidgetInstance = CreateDefaultSubobject<UWidgetComponent>(TEXT("TimerWidget"));

	fillHitBox->SetupAttachment(root);
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
}

void AGlass::BeginPlay()
{
	Super::BeginPlay();
	timerWidgetInstance->SetWidgetClass(timerWidgetClass);

	hitBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);

	sprite->SetCollisionObjectType(ECC_WorldDynamic);
	sprite->SetCollisionResponseToAllChannels(ECR_Ignore);
	sprite->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	sprite->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AGlass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdatePour();

	if (GetWorld()->GetTimerManager().IsTimerActive(glassTimer) && timerWidgetInstance)
	{
		UUserWidget *Widget = timerWidgetInstance->GetUserWidgetObject();
		if (Widget)
		{
			Widget->SetVisibility(ESlateVisibility::Visible);
			float Progress = GetWorld()->GetTimerManager().GetTimerRemaining(glassTimer) / timerDuration;
			if (UProgressBar *Bar = Cast<UProgressBar>(Widget->GetWidgetFromName(TEXT("TimerProgressBar"))))
				Bar->SetPercent(Progress);
		}
	}
	else if (timerWidgetInstance)
	{
		UUserWidget *Widget = timerWidgetInstance->GetUserWidgetObject();
		if (Widget)
			Widget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AGlass::UpdatePour()
{
	switch (GlassState) 
	{
		case EGlassState::Empty:
			TryAcquireSpout();
			break;
		case EGlassState::Pouring:
			KeepOrCancelPour();
			break;
		case EGlassState::Filled:
			break;
	}
}

void AGlass::KeepOrCancelPour()
{
	AActor *owner = pendingSpout->GetOwner();
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
		pendingSpout->KeepPouring(side);
	}
	else
	{
		CancelPour();
	}
}

void AGlass::TryAcquireSpout()
{
	AMyPlayerController *pc = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
	if (!pc)
		return;

	TArray<AActor *> over;
	fillHitBox->GetOverlappingActors(over);

	for (AActor *a : over)
	{
		if (ABlenderTop *bt = Cast<ABlenderTop>(a); bt && !pc->getIsDragging())
		{
			if (bt->getDrink() == EDrinks::noDrink)
				continue;
			pendingBlender = bt;
			Drink = bt->getDrink();
		}
		else if (AShaker *sh = Cast<AShaker>(a); sh && pc->getIsDraggingShaker())
		{
			if (sh->getDrink() == EDrinks::noDrink)
				continue;
			pendingShaker = sh;
			Drink = sh->getDrink();
		}
		else if (AIngredients *ing = Cast<AIngredients>(a); ing && pc->getIsDraggingGasoline())
		{
			if (ing->getIngredientType() != EIngredientsTypes::gasoline)
				continue;
			pendingIngredient = ing;
			Drink = EDrinks::gasoline;
		}
		else
		{
			continue;
		}

		GlassState = EGlassState::Pouring;
		pendingSpout = a->FindComponentByClass<UPouring>();
		GetWorld()->GetTimerManager().SetTimer(glassTimer, this, &AGlass::OnPourFinished, timerDuration, false);
		StartPourSound();
		return;
	}
}

void AGlass::OnPourFinished()
{
	StopPourSound();
	GlassState = EGlassState::Filled;

	sprite->SetSprite(DrinkSprites.FindRef(Drink));

	if (pendingShaker)
	{
		AShaker *shaker = pendingShaker;
		pendingShaker = nullptr;

		shaker->resetDrink();
		if (shaker->shakerOpenSprite)
			shaker->shakerOpenSprite->SetVisibility(false);
		if (shaker->GetSprite())
			shaker->GetSprite()->SetVisibility(true);
	}
	else if (pendingBlender)
	{
		ABlenderTop *blender = pendingBlender;
		pendingBlender = nullptr;
		blender->resetDrink();
	}

	pendingIngredient = nullptr;
	pendingSpout = nullptr;
}

void AGlass::CancelPour()
{
	GlassState = EGlassState::Empty;
	GetWorld()->GetTimerManager().ClearTimer(glassTimer);
	StopPourSound();
	pendingBlender = nullptr;
	pendingShaker = nullptr;
	pendingIngredient = nullptr;
	pendingSpout = nullptr;
}

void AGlass::StartPourSound()
{
	if (pourSound && !pourAudio)
		if (UMyGameInstance *GI = Cast<UMyGameInstance>(GetGameInstance()))
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

EDrinks AGlass::getDrink() const
{
	return Drink;
}
