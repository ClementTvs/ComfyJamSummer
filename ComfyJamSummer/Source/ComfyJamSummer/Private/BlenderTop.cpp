// Fill out your copyright notice in the Description page of Project Settings.


#include "BlenderTop.h"

ABlenderTop::ABlenderTop()
{
    fillHitBox = CreateDefaultSubobject<UBoxComponent>("FillHitBox");
    fillHitBox->SetupAttachment(root);

    fillHitBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
    
    fillHitBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    fillHitBox->SetCollisionObjectType(ECC_WorldDynamic);
    fillHitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    fillHitBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
    fillHitBox->SetGenerateOverlapEvents(true);
    fillHitBox->OnComponentBeginOverlap.AddDynamic(this, &ABlenderTop::OnIngredientOverlap);

    hitBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
}

const TArray<EIngredientsTypes> &ABlenderTop::getCurrentIngredients() const
{
    return currentIngredients;
}


void ABlenderTop::clearCurrentIngredients()
{
    currentIngredients.Empty();
}

void ABlenderTop::OnIngredientOverlap(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
    if (OtherComp->GetName() != TEXT("HitBox"))
        return;
    if (OtherActor && OtherActor->IsA(AIngredients::StaticClass()))
    {
        AIngredients *ingredient = Cast<AIngredients>(OtherActor);
        EIngredientsTypes ingredientType = ingredient->getIngredientType();

        if (!currentIngredients.Contains(ingredientType))
        {
            currentIngredients.Add(ingredientType);
            UE_LOG(LogTemp, Warning, TEXT("INGREDIENT ADDED"));
        }
        else
            UE_LOG(LogTemp, Warning, TEXT("INGREDIENT ALREADY THERE"));
    }
}