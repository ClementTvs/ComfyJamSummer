// Fill out your copyright notice in the Description page of Project Settings.


#include "Shaker.h"

AShaker::AShaker()
{
    PrimaryActorTick.bCanEverTick = true;

    fillHitBox = CreateDefaultSubobject<UBoxComponent>("FillHitBox");
    timerWidgetInstance = CreateDefaultSubobject<UWidgetComponent>(TEXT("TimerWidget"));
    shakerOpenSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("ShakerOpenSprite"));

    timerWidgetInstance->SetupAttachment(root);
    fillHitBox->SetupAttachment(root);
    shakerOpenSprite->SetupAttachment(root);
    
    fillHitBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    fillHitBox->SetCollisionObjectType(ECC_WorldDynamic);
    fillHitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    fillHitBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
    fillHitBox->SetGenerateOverlapEvents(true);
    fillHitBox->OnComponentBeginOverlap.AddDynamic(this, &AShaker::OnIngredientOverlap);
    fillHitBox->OnComponentEndOverlap.AddDynamic(this, &AShaker::OnIngredientEndOverlap);

    timerWidgetInstance->SetWidgetSpace(EWidgetSpace::World);
    timerWidgetInstance->SetRelativeLocation(FVector(0.f, 0.f, 40.f));
    timerWidgetInstance->SetDrawSize(FVector2D(400.f, 80.f));
    timerWidgetInstance->SetWorldScale3D(FVector(0.07f, 0.07f, 0.07f));
    timerWidgetInstance->SetWorldRotation(FRotator(0.f, -90.f, 0.f));

}

void AShaker::BeginPlay()
{
    Super::BeginPlay();

    timerWidgetInstance->SetWidgetClass(timerWidgetClass);
    pc = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
    shakerOpenSprite->SetVisibility(false);
}

void AShaker::ValidateIngredient()
{
    EIngredientsTypes ingredientType = pendingIngredient->getIngredientType();

    currentIngredients.Add(ingredientType);
    UE_LOG(LogTemp, Warning, TEXT("INGREDIENT ADDED"));
}


void AShaker::OnIngredientOverlap(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
    ABlender* blender = Cast<ABlender>(UGameplayStatics::GetActorOfClass(GetWorld(), ABlender::StaticClass()));

    if (OtherComp->GetName() != TEXT("HitBox") || drink != EDrinks::noDrink || !pc->getIsDragging() || !pc->getIsDraggingShaker())
        return;
    if (OtherActor && OtherActor->IsA(AIngredients::StaticClass()))
    {
        AIngredients *ingredient = Cast<AIngredients>(OtherActor);
        pendingIngredient = ingredient;
        EIngredientsTypes ingredientType = pendingIngredient->getIngredientType();

        canShake = true;
        if (currentIngredients.Contains(ingredientType))
        {
            UE_LOG(LogTemp, Warning, TEXT("INGREDIENT ALREADY THERE"));
        }
        else
        {
            if (ingredientType == EIngredientsTypes::gasoline)
            {
                UE_LOG(LogTemp, Warning, TEXT("PUTING GASOLINEEE..."));
                timerDuration = 3.0f;
                GetWorld()->GetTimerManager().SetTimer(IngredientTimer, this, &AShaker::ValidateIngredient, timerDuration, false);
            }
            else
            {
                timerDuration = 1.0f;
                GetWorld()->GetTimerManager().SetTimer(IngredientTimer, this, &AShaker::ValidateIngredient, timerDuration, false);
            }

        }
    }
}

void AShaker::OnIngredientEndOverlap(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
    if (OtherActor == pendingIngredient)
    {
        GetWorld()->GetTimerManager().ClearTimer(IngredientTimer);
        pendingIngredient = nullptr;
    }
}

bool AShaker::ContainsRecipe(const TArray<EIngredientsTypes>& recipe)
{
    if (recipe.Num() != currentIngredients.Num())
        return false;
    for (EIngredientsTypes ingredients : recipe)
    {
        if (!currentIngredients.Contains(ingredients))
            return false;
    }
    return true;
}

void AShaker::makeDrink()
{
    TArray<EIngredientsTypes> pinaColadaRecipe =
    {
        EIngredientsTypes::pineapple,
        EIngredientsTypes::rum,
        EIngredientsTypes::cocoJuice
    };

    if (ContainsRecipe(pinaColadaRecipe))
    {
        drink = EDrinks::pinaColada;
    }
    else
        drink = EDrinks::badDrink;
    currentIngredients.Empty();
    shakerOpenSprite->SetVisibility(true);
    sprite->SetVisibility(false);
}

EDrinks AShaker::getDrink() const
{
    return drink;
}

void AShaker::resetDrink()
{
    drink = EDrinks::noDrink;
}


void AShaker::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
 
    if (!currentIngredients.IsEmpty())
    {
        FVector2D CurrentMousePos;
        pc->GetMousePosition(CurrentMousePos.X, CurrentMousePos.Y);

        float Delta = FVector2D::Distance(CurrentMousePos, LastMousePos);
        Delta = Delta / 20.f;
        ShakePower += Delta;
        LastMousePos = CurrentMousePos;

        float ShakeThreshold = 500.f;
        if (ShakePower >= ShakeThreshold)
        {
            UE_LOG(LogTemp, Warning, TEXT("SHAKE COMPLETE!"));
            ShakePower = 0.f;
            makeDrink();
        }
    }

    ShakePower = FMath::FInterpTo(ShakePower, 0.f, DeltaTime, 0.5f);
    if (GetWorld()->GetTimerManager().IsTimerActive(IngredientTimer) && timerWidgetInstance)
    {

        UUserWidget* Widget = timerWidgetInstance->GetUserWidgetObject();

        if (Widget)
        {
            Widget->SetVisibility(ESlateVisibility::Visible);

            float Remaining = GetWorld()->GetTimerManager().GetTimerRemaining(IngredientTimer);
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