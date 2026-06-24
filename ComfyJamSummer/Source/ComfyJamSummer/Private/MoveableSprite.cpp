// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveableSprite.h"

AMoveableSprite::AMoveableSprite()
{
    root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = root;
    hitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
    sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
    hitBox->SetupAttachment(root);
    sprite->SetupAttachment(root);

    sprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    hitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    hitBox->SetCollisionObjectType(ECC_WorldDynamic);
    hitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    hitBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
    hitBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
    hitBox->SetGenerateOverlapEvents(true);
}

UPaperSpriteComponent* AMoveableSprite::GetSprite() const
{ 
    return sprite;
}

void AMoveableSprite::BeginPlay()
{
	Super::BeginPlay();
}

void AMoveableSprite::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}