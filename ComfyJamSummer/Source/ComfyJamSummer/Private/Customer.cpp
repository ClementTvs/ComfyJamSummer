// Fill out your copyright notice in the Description page of Project Settings.
#include "Customer.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"
#include "Components/BoxComponent.h"
#include "PlayerHealth.h"
#include "Glass.h"
#include "MainGameMode.h"
#include "TimerManager.h"
#include "Glass.h"

ACustomer::ACustomer()
{
    PrimaryActorTick.bCanEverTick = false;

    spriteComp = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComp"));
    RootComponent = spriteComp;
    spriteComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    serveHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ServeHitBox"));
    serveHitBox->SetupAttachment(RootComponent);
    serveHitBox->SetBoxExtent(FVector(50.f, 1000.f, 100.f));
    serveHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    serveHitBox->SetCollisionObjectType(ECC_WorldDynamic);
    serveHitBox->SetCollisionResponseToAllChannels(ECR_Overlap);
    serveHitBox->SetGenerateOverlapEvents(true);

    orderComp = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("OrderComp"));
    orderComp->SetupAttachment(RootComponent);
    orderComp->SetRelativeLocation(FVector(0.f, -1.f, 120.f));
    orderComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACustomer::BeginPlay()
{
    Super::BeginPlay();

    currentState = ECustomerState::Neutral;
    ApplyStateSprite();

    PickRandomOrder();

    serveHitBox->OnComponentBeginOverlap.AddDynamic(this, &ACustomer::OnGlassOverlap);
    serveHitBox->OnComponentEndOverlap.AddDynamic(this, &ACustomer::OnGlassLeaveCustomer);


    GetWorldTimerManager().SetTimer(patienceTimer, this,
        &ACustomer::DecreasePatience, stepInterval, true);
}

bool ACustomer::getIsOverCustomer() const
{
    return isOverCustomer;
}

void ACustomer::OnGlassLeaveCustomer(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)
{
    if (Cast<AGlass>(OtherActor))
    {
        isOverCustomer = false;
    }
}

void ACustomer::PickRandomOrder()
{
    const EDrinks options[] = {
        EDrinks::daiquiri,
        EDrinks::margarita,
		EDrinks::pinaColada
    };

    desiredDrink = options[FMath::RandRange(0, 2)];

    UE_LOG(LogTemp, Warning, TEXT("Le client commande : %d"), (int)desiredDrink);

    if (orderSprites.Contains(desiredDrink) && orderSprites[desiredDrink])
        orderComp->SetSprite(orderSprites[desiredDrink]);
}

void ACustomer::OnGlassOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (currentState == ECustomerState::Served)
        return;

    AGlass* glass = Cast<AGlass>(OtherActor);
    if (!glass)
        return;

    isOverCustomer = true;
}

void ACustomer::SellDrink()
{
    AGlass* glass = Cast<AGlass>(UGameplayStatics::GetActorOfClass(GetWorld(), AGlass::StaticClass()));
    AMainGameMode* GM = Cast<AMainGameMode>(GetWorld()->GetAuthGameMode());
	if (!glass || !GM)
		return;

    if (glass->getDrink() == EDrinks::gasoline)
    {
        UE_LOG(LogTemp, Warning, TEXT("Gasoline servie, le client meurt"));

        ReceivedWrongDrink(EDeathCause::Arrest);
        glass->Destroy();
        GetWorld()->SpawnActor<AGlass>(glassClass, spawnLocation, FRotator::ZeroRotator);
        return;
    }

    if (desiredDrink == EDrinks::pinaColada && glass->getDrink() == EDrinks::pinaColadaG
        || desiredDrink == EDrinks::margarita && glass->getDrink() == EDrinks::margaritaG
        || desiredDrink == EDrinks::daiquiri && glass->getDrink() == EDrinks::daiquiriG)
    {
        GM->OnDrinkGasolineSold();
        UE_LOG(LogTemp, Warning, TEXT("Bon cocktail servi !"));
        ReceiveDrink();
        glass->Destroy();
		GetWorld()->SpawnActor<AGlass>(glassClass, spawnLocation, FRotator::ZeroRotator);
    }


    if (glass->getDrink() == desiredDrink)
    {

        GM->OnDrinkSold();
        UE_LOG(LogTemp, Warning, TEXT("Bon cocktail servi ALIEN !"));
        ReceiveDrink();
        glass->Destroy();
		GetWorld()->SpawnActor<AGlass>(glassClass, spawnLocation, FRotator::ZeroRotator);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Mauvais cocktail (voulu=%d, recu=%d)"),
            (int)desiredDrink, (int)glass->getDrink());
		ReceivedWrongDrink(EDeathCause::Fired);
		GetWorld()->SpawnActor<AGlass>(glassClass, spawnLocation, FRotator::ZeroRotator);
		glass->Destroy();
    }
}


void ACustomer::DecreasePatience()
{
    patience = FMath::Max(0.f, patience - patienceLossPerStep);
    UpdateState();

    if (patience <= 0.f)
	{
        GetWorldTimerManager().ClearTimer(patienceTimer);
		currentState = ECustomerState::Served;

		if (receivedAngrySprite)
			spriteComp->SetSprite(receivedAngrySprite);

		if (orderComp)
			orderComp->SetVisibility(false);

        APlayerHealth* playerHealth = Cast<APlayerHealth>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerHealth::StaticClass()));
	    playerHealth->LoseLife(EDeathCause::Fired);

		StartLeaveTimer();
	}
}

void ACustomer::UpdateState()
{
    if (currentState == ECustomerState::Served)
        return;

    ECustomerState newState;
    if (patience <= angryThreshold)        newState = ECustomerState::Angry;
    else if (patience <= annoyedThreshold) newState = ECustomerState::Annoyed;
    else                                   newState = ECustomerState::Neutral;

    if (newState != currentState)
    {
        currentState = newState;
        ApplyStateSprite();
    }
}

void ACustomer::ApplyStateSprite()
{
    UPaperSprite* spriteToUse = nullptr;
    switch (currentState)
    {
    case ECustomerState::Neutral: spriteToUse = neutralSprite; break;
    case ECustomerState::Annoyed: spriteToUse = annoyedSprite; break;
    case ECustomerState::Angry:   spriteToUse = angrySprite;   break;
    default: break;
    }

    if (spriteToUse)
        spriteComp->SetSprite(spriteToUse);
}

void ACustomer::ReceiveDrink()
{
    GetWorldTimerManager().ClearTimer(patienceTimer);

    UPaperSprite* received = nullptr;
    switch (currentState)
    {
    case ECustomerState::Neutral: received = receivedNeutralSprite; break;
    case ECustomerState::Annoyed: received = receivedAnnoyedSprite; break;
    case ECustomerState::Angry:   received = receivedAngrySprite;   break;
    default: break;
    }

    currentState = ECustomerState::Served;

    if (received)
        spriteComp->SetSprite(received);

    if (orderComp)
        orderComp->SetVisibility(false);
	
	StartLeaveTimer();
}

void ACustomer::ReceivedWrongDrink(EDeathCause cause)
{
    APlayerHealth* playerHealth = Cast<APlayerHealth>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerHealth::StaticClass()));
	playerHealth->LoseLife(cause);

    GetWorldTimerManager().ClearTimer(patienceTimer);

    currentState = ECustomerState::Served;

    if (receivedAngrySprite)
        spriteComp->SetSprite(receivedAngrySprite);

    if (orderComp)
        orderComp->SetVisibility(false);

    StartLeaveTimer();
}

void ACustomer::StartLeaveTimer()
{
    const float delay = 3.f;
    GetWorldTimerManager().SetTimer(leaveTimer, this, &ACustomer::Leave, delay, false);
}

void ACustomer::Leave()
{
    if (alienSprite)
        spriteComp->SetSprite(alienSprite);

    const float destroyDelay = 0.3f;
    GetWorldTimerManager().SetTimer(leaveTimer, this, &ACustomer::FinalizeLeave, destroyDelay, false);
}

void ACustomer::FinalizeLeave()
{
    Destroy();
}