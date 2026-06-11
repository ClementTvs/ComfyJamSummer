#include "Customer.h"
#include "PaperSpriteComponent.h"
#include "Glass.h"
#include "Kismet/GameplayStatics.h"
// #include "MyGameMode.h"  // pour le compteur de morts

void ACustomer::BeginPlay()
{
    Super::BeginPlay();
    CurrentPatience = MaxPatience;
    OrderedDrink = (EDrinkType)FMath::RandRange(1, 3);
    if (NormalSprite) 
		SpriteComp->SetSprite(NormalSprite);
}

void ACustomer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (bServed) 
		return;

    CurrentPatience -= DeltaTime;
    if (CurrentPatience <= 0.f)
        Leave();
}

void ACustomer::ReceiveGlass(AGlass* Glass)
{
    if (bServed || !Glass) 
		return;

    if (Glass->IsOnlyGasoline())
    {
        Die();
    }
    else if (Glass->ContainsGasoline())
    {
        Serve();
    }
	else if (Glass->ContainsGasoline())
    // TODO: cas "cocktail + essence" et "mauvais cocktail" → à définir
}

void ACustomer::Serve()
{
    bServed = true;
    if (SatisfiedSprite) SpriteComp->SetSprite(SatisfiedSprite); // devient vert
    // TODO: score, le client part après un délai...
}

void ACustomer::Die()
{
    bServed = true;
    if (DeadSprite) SpriteComp->SetSprite(DeadSprite);
    // TODO: prévenir le GameMode → +1 mort → check les 3 endings
}

void ACustomer::Leave()
{
    bServed = true;
    // TODO: client part fâché (conséquence à définir)
}