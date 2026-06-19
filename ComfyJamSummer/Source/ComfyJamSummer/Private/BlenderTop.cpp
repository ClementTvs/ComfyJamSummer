#include "BlenderTop.h"
#include "Blender.h"
#include "Glass.h"
#include "MyPlayerController.h"
#include "Pouring.h"

ABlenderTop::ABlenderTop()
{
    PrimaryActorTick.bCanEverTick = true;

    fillHitBox = CreateDefaultSubobject<UBoxComponent>("FillHitBox");
    timerWidgetInstance = CreateDefaultSubobject<UWidgetComponent>(TEXT("TimerWidget"));
    pourSpout = CreateDefaultSubobject<UPouring>(TEXT("PourSpout"));

    fillHitBox->SetupAttachment(root);
    timerWidgetInstance->SetupAttachment(root);
	timerWidgetInstance->SetWidgetSpace(EWidgetSpace::World);
	timerWidgetInstance->SetRelativeLocation(FVector(0.f, 0.f, 80.f));
	timerWidgetInstance->SetDrawSize(FVector2D(400.f, 80.f));
	timerWidgetInstance->SetRelativeScale3D(FVector(0.07f, 0.07f, 0.07f));
	timerWidgetInstance->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
    
    fillHitBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    fillHitBox->SetCollisionObjectType(ECC_WorldDynamic);
    fillHitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    fillHitBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
    fillHitBox->SetGenerateOverlapEvents(true);
    fillHitBox->OnComponentBeginOverlap.AddDynamic(this, &ABlenderTop::OnIngredientOverlap);
    fillHitBox->OnComponentEndOverlap.AddDynamic(this, &ABlenderTop::OnIngredientEndOverlap);

    hitBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
}

void ABlenderTop::BeginPlay()
{
    Super::BeginPlay();
    timerWidgetInstance->SetWidgetClass(timerWidgetClass);

    fillHitBox->SetUsingAbsoluteRotation(true);
    hitBox->SetUsingAbsoluteRotation(true);

    if (pourSpout)
    {
        pourSpout->bFixedDirection = true;
        pourSpout->fixedDirection = 1.f;
    }
}

const TArray<EIngredientsTypes> &ABlenderTop::getCurrentIngredients() const { return currentIngredients; }
void ABlenderTop::clearCurrentIngredients() { currentIngredients.Empty(); }

void ABlenderTop::ValidateIngredient()
{
    if (!pendingIngredient)
        return;

    currentIngredients.Add(pendingIngredient->getIngredientType());
    UE_LOG(LogTemp, Warning, TEXT("INGREDIENT ADDED"));

    pendingIngredient = nullptr;
    pendingIngredientSpout = nullptr;
}

void ABlenderTop::CancelIngredientPour()
{
    GetWorld()->GetTimerManager().ClearTimer(IngredientTimer);
    pendingIngredient = nullptr;
    pendingIngredientSpout = nullptr;
}

void ABlenderTop::resetDrink() { drink = EDrinks::noDrink; }
void ABlenderTop::setDrink(EDrinks newDrink) { drink = newDrink; }
EDrinks ABlenderTop::getDrink() const { return drink; }

void ABlenderTop::OnIngredientEndOverlap(UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor == pendingIngredient)
        CancelIngredientPour();
}

void ABlenderTop::OnIngredientOverlap(UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AMyPlayerController *pc = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
    ABlender* blender = Cast<ABlender>(UGameplayStatics::GetActorOfClass(GetWorld(), ABlender::StaticClass()));

    if (OtherComp->GetName() != TEXT("HitBox") || !blender || blender->IsBlenderWorking()
        || drink != EDrinks::noDrink || !pc->getIsDragging())
        return;

    if (OtherActor && OtherActor->IsA(AIngredients::StaticClass()))
    {
        AIngredients *ingredient = Cast<AIngredients>(OtherActor);
        EIngredientsTypes ingredientType = ingredient->getIngredientType();

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
                &ABlenderTop::ValidateIngredient, timerDuration, false);
        }
    }
}

void ABlenderTop::Tick(float DeltaTime)
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