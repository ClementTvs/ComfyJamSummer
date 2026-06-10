// Fill out your copyright notice in the Description page of Project Settings.


#include "Blender.h"

ABlender::ABlender()
{
    root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = root;

    hitBox = CreateDefaultSubobject<UBoxComponent>("HitBox");
    blenderSprite = CreateDefaultSubobject<UPaperSpriteComponent>("BlenderSprite");

    hitBox->SetupAttachment(root);
    blenderSprite->SetupAttachment(root);

    hitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    hitBox->SetCollisionObjectType(ECC_WorldStatic);
    hitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    hitBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
    hitBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
    hitBox->SetGenerateOverlapEvents(true);
    hitBox->OnClicked.AddDynamic(this, &ABlender::OnBlenderClicked);
    // hitBox->OnComponentBeginOverlap.AddDynamic(this, &ABlender::OnTopTouchBottom);
    hitBox->OnComponentBeginOverlap.AddDynamic(this, &ABlender::OnTopEnter);
    hitBox->OnComponentEndOverlap.AddDynamic(this, &ABlender::OnTopLeaveBottom);
}

void ABlender::BeginPlay()
{
    Super::BeginPlay();

    blenderTopRef = Cast<ABlenderTop>(UGameplayStatics::GetActorOfClass(GetWorld(), ABlenderTop::StaticClass()));
}

bool ABlender::IsOverBlender() const
{
    return isOverBlender;
}

bool ABlender::ContainsRecipe(const TArray<EIngredientsTypes>& recipe)
{
    const TArray<EIngredientsTypes>& allIngredients = blenderTopRef->getCurrentIngredients();
    if (recipe.Num() != allIngredients.Num())
        return false;
    for (EIngredientsTypes ingredients : recipe)
    {
        if (!allIngredients.Contains(ingredients))
            return false;
    }
    return true;
}

// void ABlender::OnTopTouchBottom(UPrimitiveComponent* OverlappedComp,
// 	AActor* OtherActor,
// 	UPrimitiveComponent* OtherComp,
// 	int32 OtherBodyIndex,
// 	bool bFromSweep,
// 	const FHitResult& SweepResult)
// {
//     UE_LOG(LogTemp, Warning, TEXT("TOUCHEEE"));
// }

void ABlender::OnBlenderClicked(UPrimitiveComponent* ClickedComp, FKey ButtonPressed)
{
    TArray<EIngredientsTypes> pinaColadaRecipe =
    {
        EIngredientsTypes::ananas,
        EIngredientsTypes::rhum,
        EIngredientsTypes::cocoMilk
    };

    if (ButtonPressed == EKeys::LeftMouseButton || ButtonPressed == EKeys::RightMouseButton)
    {
        if (blenderTopRef->getCurrentIngredients().IsEmpty())
        {
            UE_LOG(LogTemp, Warning, TEXT("CEST VIDE"));
            return ;
        }
        else if (ContainsRecipe(pinaColadaRecipe))
        {
            UE_LOG(LogTemp, Warning, TEXT("PINA COLADA PRETE !!!"));
        }
        else
            UE_LOG(LogTemp, Warning, TEXT("CEST QUOI CE TRUC"));
        blenderTopRef->clearCurrentIngredients();
    }
}

void ABlender::OnTopEnter(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (Cast<ABlenderTop>(OtherActor))
    {
        isOverBlender = true;
    }
}

void ABlender::OnTopLeaveBottom(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)
{

    if (Cast<ABlenderTop>(OtherActor))
    {
        isOverBlender = false;
    }
}