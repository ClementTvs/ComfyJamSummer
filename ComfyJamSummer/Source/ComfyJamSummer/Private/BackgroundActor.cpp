#include "BackgroundActor.h"

ABackgroundActor::ABackgroundActor()
{
    PrimaryActorTick.bCanEverTick = false;

    baseBackground = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("baseBackground"));
    RootComponent = baseBackground;

    alien1 = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("alien1"));
    alien1->SetupAttachment(RootComponent);

    alien2 = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("alien2"));
    alien2->SetupAttachment(RootComponent);

    alien3 = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("alien3"));
    alien3->SetupAttachment(RootComponent);
}

void ABackgroundActor::BeginPlay()
{
    Super::BeginPlay();

    alien1->SetVisibility(false);
    alien2->SetVisibility(false);
    alien3->SetVisibility(false);
}

void ABackgroundActor::UpdateBackground(int32 DrinksSold)
{
    if (DrinksSold >= 3 && alien1)
        alien1->SetVisibility(true);

    if (DrinksSold >= 6 && alien2)
        alien2->SetVisibility(true);

    if (DrinksSold >= 9 && alien3)
        alien3->SetVisibility(true);
}