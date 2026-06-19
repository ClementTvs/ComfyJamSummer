// Fill out your copyright notice in the Description page of Project Settings.


#include "Blender.h"

ABlender::ABlender()
{
    PrimaryActorTick.bCanEverTick = true;

    root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = root;

    hitBox = CreateDefaultSubobject<UBoxComponent>("HitBox");
    blenderSprite = CreateDefaultSubobject<UPaperSpriteComponent>("BlenderSprite");
    timerWidgetInstance = CreateDefaultSubobject<UWidgetComponent>(TEXT("TimerWidget"));


    hitBox->SetupAttachment(root);
    blenderSprite->SetupAttachment(root);
    timerWidgetInstance->SetupAttachment(root);
	timerWidgetInstance->SetWidgetSpace(EWidgetSpace::World);
	timerWidgetInstance->SetRelativeLocation(FVector(0.f, 0.f, 80.f));
	timerWidgetInstance->SetDrawSize(FVector2D(400.f, 80.f));
	timerWidgetInstance->SetRelativeScale3D(FVector(0.07f, 0.07f, 0.07f));
	timerWidgetInstance->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

    hitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    hitBox->SetCollisionObjectType(ECC_WorldStatic);
    hitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    hitBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
    hitBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
    hitBox->SetGenerateOverlapEvents(true);
    hitBox->OnClicked.AddDynamic(this, &ABlender::OnBlenderClicked);
    hitBox->OnComponentBeginOverlap.AddDynamic(this, &ABlender::OnTopEnter);
    hitBox->OnComponentEndOverlap.AddDynamic(this, &ABlender::OnTopLeaveBottom);

}

void ABlender::BeginPlay()
{
    Super::BeginPlay();

    blenderTopRef = Cast<ABlenderTop>(UGameplayStatics::GetActorOfClass(GetWorld(), ABlenderTop::StaticClass()));
    isBlenderFusion = true;
    timerWidgetInstance->SetWidgetClass(timerWidgetClass);
}

bool ABlender::IsOverBlender() const
{
    return isOverBlender;
}

bool ABlender::IsBlenderFusion() const
{
    return isBlenderFusion;
}

bool ABlender::IsBlenderWorking() const
{
    return isBlenderWorking;
}

void ABlender::isBlenderFusionFalse()
{
    isBlenderFusion = false;
    GetWorld()->GetTimerManager().ClearTimer(blenderTimer);
    isBlenderWorking = false;
    StopBlenderSound();
    UE_LOG(LogTemp, Warning, TEXT("BLENDER ARRETE"));
}

void ABlender::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (GetWorld()->GetTimerManager().IsTimerActive(blenderTimer) && timerWidgetInstance)
    {

        UUserWidget* Widget = timerWidgetInstance->GetUserWidgetObject();
        if (Widget)
        {
            Widget->SetVisibility(ESlateVisibility::Visible);

            float Remaining = GetWorld()->GetTimerManager().GetTimerRemaining(blenderTimer);
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

void ABlender::BlenderStart()
{
    static const TMap<EDrinks, TArray<EIngredientsTypes>> Recipes =
    {
        { EDrinks::daiquiri,   { EIngredientsTypes::red, EIngredientsTypes::blue } },
        { EDrinks::margarita,  { EIngredientsTypes::red, EIngredientsTypes::yellow } },
        { EDrinks::daiquiriG,   { EIngredientsTypes::red, EIngredientsTypes::blue, EIngredientsTypes::gasoline } },
        { EDrinks::margaritaG,  { EIngredientsTypes::red, EIngredientsTypes::yellow, EIngredientsTypes::gasoline } },
		{ EDrinks::gasoline, { EIngredientsTypes::gasoline }}
    };

    EDrinks result = EDrinks::badDrink;
    for (const TPair<EDrinks, TArray<EIngredientsTypes>>& R : Recipes)
    {
        if (ContainsRecipe(R.Value))
        {
            result = R.Key;
            break;
        }
    }

    blenderTopRef->setDrink(result);
    blenderTopRef->clearCurrentIngredients();
    isBlenderWorking = false;
	StopBlenderSound();
}

void ABlender::OnBlenderClicked(UPrimitiveComponent* ClickedComp, FKey ButtonPressed)
{
    if (ButtonPressed == EKeys::LeftMouseButton && isBlenderFusion)
    {
        if (blenderTopRef->getCurrentIngredients().IsEmpty())
        {
            UE_LOG(LogTemp, Warning, TEXT("CEST VIDE"));
            return ;
        }
        else
        {
            isBlenderWorking = true;
            UE_LOG(LogTemp, Warning, TEXT("BRRRRRR..."));
            timerDuration = 5.0f;
            GetWorld()->GetTimerManager().SetTimer(blenderTimer, this, &ABlender::BlenderStart, 5.0f, false);
        	StartBlenderSound();
		}
    }
    else
        UE_LOG(LogTemp, Warning, TEXT("BLENDER MISSING"));

}

void ABlender::FusionBlender()
{
    isBlenderFusion = true;

    TArray<AActor*> overlappingActors;
    GetOverlappingActors(overlappingActors, ABlenderTop::StaticClass());

    if (overlappingActors.Num() == 0)
        return ;
    
    ABlenderTop *blenderTop = Cast<ABlenderTop>(overlappingActors[0]);

    if (!blenderTop)
        return ;
    
    FVector blenderTopLocation = GetActorLocation();
    blenderTopLocation.Z += 31;
    blenderTop->SetActorLocation(blenderTopLocation);
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
        if (isBlenderFusion)
        {
            isBlenderFusion = false;
            GetWorld()->GetTimerManager().ClearTimer(blenderTimer);
            isBlenderWorking = false;
            StopBlenderSound();
        }
        isOverBlender = false;
    }
}

void ABlender::StartBlenderSound()
{
    if (blenderSound && !blenderAudio)
        if (UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance()))
            blenderAudio = GI->SpawnSFX(blenderSound, 0.5f);
}

void ABlender::StopBlenderSound()
{
    if (blenderAudio)
    {
        blenderAudio->Stop();
        blenderAudio = nullptr;
    }
}