// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHealth.h"

// Sets default values
APlayerHealth::APlayerHealth()
{
    root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = root;
    for (int32 i = 0; i < 3; i++)
    {
        FName FullName = *FString::Printf(TEXT("HeartFull_%d"), i);
        FName EmptyName = *FString::Printf(TEXT("HeartEmpty_%d"), i);

        UPaperSpriteComponent* Full = CreateDefaultSubobject<UPaperSpriteComponent>(FullName);
        UPaperSpriteComponent* Empty = CreateDefaultSubobject<UPaperSpriteComponent>(EmptyName);

        Full->SetupAttachment(root);
        Empty->SetupAttachment(root);
        
        Empty->SetHiddenInGame(true);

        heartsFullArray.Add(Full);
        heartsEmptyArray.Add(Empty);
    }
}


void APlayerHealth::LoseLife()
{
    int liveLost = 3 - lives;

    if (lives > 0)
        lives--;
    else
        return ;

    heartsFullArray[liveLost]->SetHiddenInGame(true);
    heartsEmptyArray[liveLost]->SetHiddenInGame(false);
    if (lives == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("YOU LOST"));
        return;
    }
}
