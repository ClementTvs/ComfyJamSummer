// Fill out your copyright notice in the Description page of Project Settings.


#include "Glass.h"

AGlass::AGlass()
{
    fillHitBox = CreateDefaultSubobject<UBoxComponent>("FillHitBox");
    fillHitBox->SetupAttachment(root);

    fillHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    fillHitBox->SetCollisionObjectType(ECC_WorldStatic);
    fillHitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    fillHitBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
    fillHitBox->SetGenerateOverlapEvents(true);
    // fillHitBox->OnComponentBeginOverlap.AddDynamic(this, &AGlass::OnIngredientOverlap);
}
