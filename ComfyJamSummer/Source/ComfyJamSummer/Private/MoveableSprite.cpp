// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveableSprite.h"

// Sets default values
AMoveableSprite::AMoveableSprite()
{
    root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = root;
    moveable = CreateDefaultSubobject<USceneComponent>(TEXT("Moveable"));
    moveable->SetupAttachment(root);
    hitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
    sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
    hitBox->SetupAttachment(moveable);
    sprite->SetupAttachment(moveable);

    // Le sprite ne doit pas intercepter le clic à la place du hitBox
    sprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // hitBox = à la fois cliquable (Visibility) et overlap avec le fillHitBox du blender
    hitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    hitBox->SetCollisionObjectType(ECC_WorldDynamic);
    hitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    hitBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);    // pour GetHitResultUnderCursor
    hitBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap); // pour overlaper le blender
    hitBox->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AMoveableSprite::BeginPlay()
{
	Super::BeginPlay();
}

USceneComponent *AMoveableSprite::GetMoveable() const
{
    return moveable; 
}

void AMoveableSprite::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}