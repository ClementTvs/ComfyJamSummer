// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveableSprite.h"

// Sets default values
AMoveableSprite::AMoveableSprite()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = root;

	hitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));

	hitBox->SetupAttachment(root);	
	sprite->SetupAttachment(root);	

}

// Called when the game starts or when spawned
void AMoveableSprite::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMoveableSprite::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}