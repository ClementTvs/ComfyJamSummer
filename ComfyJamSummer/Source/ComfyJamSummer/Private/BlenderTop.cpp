// Fill out your copyright notice in the Description page of Project Settings.

#include "BlenderTop.h"
#include "Blender.h"
#include "Glass.h"
#include "MyPlayerController.h"

ABlenderTop::ABlenderTop()
{
    PrimaryActorTick.bCanEverTick = true;

    fillHitBox = CreateDefaultSubobject<UBoxComponent>("FillHitBox");
    timerWidgetInstance = CreateDefaultSubobject<UWidgetComponent>(TEXT("TimerWidget"));
    fillHitBox->SetupAttachment(root);
    timerWidgetInstance->SetupAttachment(root);
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
}


const TArray<EIngredientsTypes> &ABlenderTop::getCurrentIngredients() const
{
    return currentIngredients;
}


void ABlenderTop::clearCurrentIngredients()
{
    currentIngredients.Empty();
}

void ABlenderTop::ValidateIngredient()
{
    pendingIngredient->StopPouring();
    if (!pendingIngredient)
        return ;

    EIngredientsTypes ingredientType = pendingIngredient->getIngredientType();

    currentIngredients.Add(ingredientType);
    UE_LOG(LogTemp, Warning, TEXT("INGREDIENT ADDED"));
}

void ABlenderTop::resetDrink()
{
    drink = EDrinks::noDrink;
}

void ABlenderTop::setDrink(EDrinks newDrink)
{
	drink = newDrink;
}

EDrinks ABlenderTop::getDrink() const
{
    return drink;
}

void ABlenderTop::OnIngredientEndOverlap(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
    if (OtherActor == pendingIngredient)
    {
        pendingIngredient->StopPouring();
        GetWorld()->GetTimerManager().ClearTimer(IngredientTimer);
        pendingIngredient = nullptr;
    }
}

void ABlenderTop::OnIngredientOverlap(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
    AMyPlayerController *pc = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
    ABlender* blender = Cast<ABlender>(UGameplayStatics::GetActorOfClass(GetWorld(), ABlender::StaticClass()));
    
    if (OtherComp->GetName() != TEXT("HitBox") || !blender || blender->IsBlenderWorking() || drink != EDrinks::noDrink || !pc->getIsDragging())
        return;
    if (OtherActor && OtherActor->IsA(AIngredients::StaticClass()))
    {
        AIngredients *ingredient = Cast<AIngredients>(OtherActor);
        pendingIngredient = ingredient;

        EIngredientsTypes ingredientType = pendingIngredient->getIngredientType();

        if (currentIngredients.Contains(ingredientType))
        {
            UE_LOG(LogTemp, Warning, TEXT("INGREDIENT ALREADY THERE"));
        }
        else
        {
            bool bTiltLeft = ingredient->GetActorLocation().X > GetActorLocation().X;
            ingredient->StartPouring(bTiltLeft);
            if (ingredientType == EIngredientsTypes::gasoline)
            {
                UE_LOG(LogTemp, Warning, TEXT("PUTING GASOLINEEE..."));
                timerDuration = 3.0f;
                GetWorld()->GetTimerManager().SetTimer(IngredientTimer, this, &ABlenderTop::ValidateIngredient, timerDuration, false);
            }
            else
            {
                timerDuration = 1.0f;
                GetWorld()->GetTimerManager().SetTimer(IngredientTimer, this, &ABlenderTop::ValidateIngredient, timerDuration, false);
            }

        }
    }
}

void ABlenderTop::StartPouring()
{
    isPouring = true;
}

void ABlenderTop::StopPouring()
{
    isPouring = false;
    SetActorRotation(FRotator(0.f, 0.f, 0.f));
}

void ABlenderTop::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (isPouring)
    {
        TArray<AActor*> overlappingActors;
        GetOverlappingActors(overlappingActors);
        
        bool stillOverlapping = false;
        for (AActor* actor : overlappingActors)
        {
            if (actor->IsA(AGlass::StaticClass()))
            {
                stillOverlapping = true;
                break;
            }
        }
        
        if (!stillOverlapping)
        {
            StopPouring();
        } else {
			FRotator CurrentRotation = GetActorRotation();
			float TargetPitch = -70.f;
			CurrentRotation.Pitch = FMath::FInterpTo(CurrentRotation.Pitch, TargetPitch, DeltaTime, 3.f);
			SetActorRotation(CurrentRotation);
		}
        
    }
    if (GetWorld()->GetTimerManager().IsTimerActive(IngredientTimer) && timerWidgetInstance)
    {

        UUserWidget* Widget = timerWidgetInstance->GetUserWidgetObject();

        if (Widget)
        {
            Widget->SetVisibility(ESlateVisibility::Visible);

            float Remaining = GetWorld()->GetTimerManager().GetTimerRemaining(IngredientTimer);
            float Progress = Remaining / timerDuration;

            UProgressBar* Bar = Cast<UProgressBar>(Widget->GetWidgetFromName(TEXT("TimerProgressBar")));

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
