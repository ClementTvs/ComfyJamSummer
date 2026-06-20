#include "Ingredients.h"
#include "Pouring.h"
#include "Kismet/GameplayStatics.h"
#include "Shaker.h"
#include "Glass.h"
#include "BlenderTop.h"
#include "Alien.h"

void AIngredients::BeginPlay()
{
    Super::BeginPlay();

	hitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	hitBox->SetCollisionObjectType(ECC_WorldDynamic);
	hitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	hitBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);  // <-- Block devient Ignore : la box ne capte plus le clic
	hitBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	hitBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	hitBox->SetUsingAbsoluteRotation(true);
	hitBox->SetGenerateOverlapEvents(true);

	// Le clic passe sur le polygone du sprite (et SEULEMENT le clic)
	sprite->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	sprite->SetCollisionObjectType(ECC_WorldDynamic);
	sprite->SetCollisionResponseToAllChannels(ECR_Ignore);
	sprite->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	sprite->SetGenerateOverlapEvents(false); // surtout pas d'overlap, sinon ça pollue le pour

	if (pourSpout)
	{
		pourSpout->directionSign = -1.f;

		if (ingredientType == EIngredientsTypes::gasoline)
		{
			pourSpout->bFixedDirection = true;
			pourSpout->fixedDirection = 1.f;
		}
	}
}

AIngredients::AIngredients()
{
    PrimaryActorTick.bCanEverTick = true;

    fillSprite = CreateDefaultSubobject<UPaperSpriteComponent>("FillSprite");
    fillSprite->SetupAttachment(root);
    fillSprite->SetHiddenInGame(true);

    pourSpout = CreateDefaultSubobject<UPouring>(TEXT("PourSpout"));

    hitBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    hitBox->SetCollisionObjectType(ECC_WorldDynamic);
    hitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    hitBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
    hitBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
    hitBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
    hitBox->SetGenerateOverlapEvents(true);
}

const EIngredientsTypes &AIngredients::getIngredientType() const
{
    return ingredientType;
}

void AIngredients::OnGrabbed()
{
    if (ingredientType == EIngredientsTypes::gasoline)
    {
        if (AAlien* Alien = Cast<AAlien>(
            UGameplayStatics::GetActorOfClass(GetWorld(), AAlien::StaticClass())))
        {
            Alien->OnGasolineGrab();
            Alien->bGasolineInUse = true;
        }
    }
}

void AIngredients::OnReleased()
{
    if (ingredientType == EIngredientsTypes::gasoline)
    {
        if (AAlien* Alien = Cast<AAlien>(
            UGameplayStatics::GetActorOfClass(GetWorld(), AAlien::StaticClass())))
        {
            Alien->bGasolineInUse = false;
        }
    }
}

void AIngredients::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}