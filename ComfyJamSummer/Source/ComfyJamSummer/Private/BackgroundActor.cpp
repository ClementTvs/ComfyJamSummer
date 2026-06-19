#include "BackgroundActor.h"

ABackgroundActor::ABackgroundActor()
{
    PrimaryActorTick.bCanEverTick = false;

    USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = root;
    
    baseBackground = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("baseBackground"));
    baseBackground->SetupAttachment(RootComponent);

    alien1 = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("alien1"));
    alien1->SetupAttachment(RootComponent);

    alien2 = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("alien2"));
    alien2->SetupAttachment(RootComponent);

    alien3 = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("alien3"));
    alien3->SetupAttachment(RootComponent);

    deadMan = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("deadMan"));
    deadMan->SetupAttachment(RootComponent);
}

void ABackgroundActor::BeginPlay()
{
    Super::BeginPlay();

    alien1->SetVisibility(false);
    alien2->SetVisibility(false);
    alien3->SetVisibility(false);
    deadMan->SetVisibility(false);
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

void ABackgroundActor::DeadBody()
{
    deadMan->SetVisibility(true);
}