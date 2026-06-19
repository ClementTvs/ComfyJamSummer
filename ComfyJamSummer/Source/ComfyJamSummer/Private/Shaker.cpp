#include "Glass.h"
#include "Shaker.h"
#include "Pouring.h"

AShaker::AShaker()
{
    PrimaryActorTick.bCanEverTick = true;

    fillHitBox = CreateDefaultSubobject<UBoxComponent>("FillHitBox");
    timerWidgetInstance = CreateDefaultSubobject<UWidgetComponent>(TEXT("TimerWidget"));
    shakerOpenSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("ShakerOpenSprite"));
    pourSpout = CreateDefaultSubobject<UPouring>(TEXT("PourSpout"));

    timerWidgetInstance->SetupAttachment(root);
    fillHitBox->SetupAttachment(root);
    shakerOpenSprite->SetupAttachment(root);

    timerWidgetInstance->SetWidgetSpace(EWidgetSpace::World);
    timerWidgetInstance->SetRelativeLocation(FVector(0.f, 0.f, 40.f));
    timerWidgetInstance->SetDrawSize(FVector2D(400.f, 80.f));
    timerWidgetInstance->SetWorldScale3D(FVector(0.07f, 0.07f, 0.07f));
    timerWidgetInstance->SetWorldRotation(FRotator(0.f, -90.f, 0.f));
    fillHitBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    fillHitBox->SetCollisionObjectType(ECC_WorldDynamic);
    fillHitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    fillHitBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
    fillHitBox->SetGenerateOverlapEvents(true);
    fillHitBox->OnComponentBeginOverlap.AddDynamic(this, &AShaker::OnIngredientOverlap);
    fillHitBox->OnComponentEndOverlap.AddDynamic(this, &AShaker::OnIngredientEndOverlap);
}

void AShaker::BeginPlay()
{
    Super::BeginPlay();

    timerWidgetInstance->SetWidgetClass(timerWidgetClass);
    pc = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
    shakerOpenSprite->SetVisibility(false);

    fillHitBox->SetUsingAbsoluteRotation(true);
    hitBox->SetUsingAbsoluteRotation(true);
}

void AShaker::OnIngredientOverlap(UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherComp->GetName() != TEXT("HitBox") || drink != EDrinks::noDrink
        || !pc->getIsDragging() || pc->getIsDraggingShaker())
        return;

    if (OtherActor && OtherActor->IsA(AIngredients::StaticClass()))
    {
        AIngredients *ingredient = Cast<AIngredients>(OtherActor);
        EIngredientsTypes ingredientType = ingredient->getIngredientType();

        canShake = true;
        if (currentIngredients.Contains(ingredientType))
        {
            UE_LOG(LogTemp, Warning, TEXT("INGREDIENT ALREADY THERE"));
        }
        else
        {
            pendingIngredient = ingredient;
            pendingIngredientSpout = ingredient->FindComponentByClass<UPouring>();

            timerDuration = (ingredientType == EIngredientsTypes::gasoline) ? 3.0f : 1.0f;
            GetWorld()->GetTimerManager().SetTimer(IngredientTimer, this,
                &AShaker::ValidateIngredient, timerDuration, false);
        }
    }
}

void AShaker::OnIngredientEndOverlap(UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor == pendingIngredient)
        CancelIngredientPour();
}

void AShaker::CancelIngredientPour()
{
    GetWorld()->GetTimerManager().ClearTimer(IngredientTimer);
    pendingIngredient = nullptr;
    pendingIngredientSpout = nullptr;
}

void AShaker::ValidateIngredient()
{
    if (!pendingIngredient)
        return;

    currentIngredients.Add(pendingIngredient->getIngredientType());
    UE_LOG(LogTemp, Warning, TEXT("INGREDIENT ADDED"));

    pendingIngredient = nullptr;
    pendingIngredientSpout = nullptr;
}

bool AShaker::ContainsRecipe(const TArray<EIngredientsTypes>& recipe)
{
    if (recipe.Num() != currentIngredients.Num())
        return false;
    for (EIngredientsTypes ingredients : recipe)
        if (!currentIngredients.Contains(ingredients))
            return false;
    return true;
}

void AShaker::makeDrink()
{
    TArray<EIngredientsTypes> pinaColadaRecipe = { EIngredientsTypes::yellow, EIngredientsTypes::blue };
    TArray<EIngredientsTypes> pinaColadaGasolineRecipe = { EIngredientsTypes::yellow, EIngredientsTypes::blue, EIngredientsTypes::gasoline };
    TArray<EIngredientsTypes> straightGasoline = { EIngredientsTypes::gasoline };

    if (ContainsRecipe(pinaColadaRecipe))
		drink = EDrinks::pinaColada;
    else if (ContainsRecipe(straightGasoline))
		drink = EDrinks::gasoline;
    else if (ContainsRecipe(pinaColadaGasolineRecipe))
		drink = EDrinks::pinaColadaG;
    else
		drink = EDrinks::badDrink;

    currentIngredients.Empty();
    shakerOpenSprite->SetVisibility(true);
    sprite->SetVisibility(false);
}

EDrinks AShaker::getDrink() const { return drink; }
void AShaker::resetDrink() { drink = EDrinks::noDrink; }

void AShaker::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (pendingIngredient && pendingIngredientSpout)
    {
		const FVector d = pendingIngredient->GetActorLocation() - GetActorLocation();
		const float dist = FVector(d.X, 0.f, d.Z).Size();

		if (dist <= fillMaxDistance)
		{
			const float side = (pendingIngredient->GetActorLocation().X > GetActorLocation().X) ? 1.f : -1.f;
			pendingIngredientSpout->KeepPouring(side);
		}
		else
		{
			CancelIngredientPour();
		}
    }

    if (!currentIngredients.IsEmpty() && pc && pc->getIsDraggingShaker())
    {
        FVector2D CurrentMousePos;
        pc->GetMousePosition(CurrentMousePos.X, CurrentMousePos.Y);

        float Delta = FVector2D::Distance(CurrentMousePos, LastMousePos);
        Delta = Delta / 125.f;
        ShakePower += Delta;
        LastMousePos = CurrentMousePos;

        StartShakerSound();

        if (ShakePower >= 50.f)
        {
            UE_LOG(LogTemp, Warning, TEXT("SHAKE COMPLETE!"));
            ShakePower = 0.f;
            makeDrink();
        }
    }
    else
    {
        StopShakerSound();
    }

    ShakePower = FMath::FInterpTo(ShakePower, 0.f, DeltaTime, 0.0005f);

    if (GetWorld()->GetTimerManager().IsTimerActive(IngredientTimer) && timerWidgetInstance)
    {
        UUserWidget* Widget = timerWidgetInstance->GetUserWidgetObject();
        if (Widget)
        {
            Widget->SetVisibility(ESlateVisibility::Visible);
            float Progress = GetWorld()->GetTimerManager().GetTimerRemaining(IngredientTimer) / timerDuration;
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

void AShaker::StartShakerSound()
{
    if (shakerSound && !shakerAudio)
        if (UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance()))
            shakerAudio = GI->SpawnSFX(shakerSound, 3.f);
}

void AShaker::StopShakerSound()
{
    if (shakerAudio)
    {
        shakerAudio->Stop();
        shakerAudio = nullptr;
    }
}