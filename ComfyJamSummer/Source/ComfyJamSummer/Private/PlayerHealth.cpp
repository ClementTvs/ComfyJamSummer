
#include "MyGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "BackgroundActor.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerHealth.h"

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

void APlayerHealth::LoseLife(EDeathCause cause)
{
    if (lives <= 0)
        return;

    if (cause == EDeathCause::Arrest)
        arrestCount++;
    else if (cause == EDeathCause::Fired)
        firedCount++;

    lives--;
    UpdateHearts();
    if (arrestCount == 2)
    {
        UE_LOG(LogTemp, Warning, TEXT("OUI"))
        BackgroundActorRef = Cast<ABackgroundActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ABackgroundActor::StaticClass()));
	    if (BackgroundActorRef)
        {
            UE_LOG(LogTemp, Warning, TEXT("RENTRE"))
		    BackgroundActorRef->DeadBody();
        }
    }

    if (lockedEnding == EDeathCause::None)
    {
        if (arrestCount >= strikesToLose)
            lockedEnding = EDeathCause::Arrest;
        else if (firedCount >= strikesToLose)
            lockedEnding = EDeathCause::Fired;
    }

    if (lives <= 0)
    {
        if (lockedEnding == EDeathCause::None)
            lockedEnding = (arrestCount > firedCount) ? EDeathCause::Arrest : EDeathCause::Fired;

        if (lockedEnding == EDeathCause::Arrest)
        {
            policeEnd();
        }
        else
        {
            firedEnd();
        }
    }
}

void APlayerHealth::policeEnd()
{
    if (policeEndingScreen)
    {
        arrestCount = 0;
        firedCount = 0;
        lockedEnding = EDeathCause::None;
        UUserWidget* EndScreen = CreateWidget<UUserWidget>(GetWorld(), policeEndingScreen);
        EndScreen->AddToViewport();
        UGameplayStatics::SetGamePaused(this, true);
        
        UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
        if (GI)
            GI->PlayMusic(GI->badEndingMusic);
    }
}

void APlayerHealth::firedEnd()
{
    if (firedEndingScreen)
    {
        arrestCount = 0;
        firedCount = 0;
        lockedEnding = EDeathCause::None;
        UUserWidget* EndScreen = CreateWidget<UUserWidget>(GetWorld(), firedEndingScreen);
        EndScreen->AddToViewport();
        UGameplayStatics::SetGamePaused(this, true);
        
        UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
        if (GI)
            GI->PlayMusic(GI->badEndingMusic);
    }
}

void APlayerHealth::UpdateHearts()
{
    int totalLost = heartsFullArray.Num() - lives;
    for (int32 i = 0; i < heartsFullArray.Num(); i++)
    {
        bool lost = i < totalLost;
        heartsFullArray[i]->SetHiddenInGame(lost);
        if (heartsEmptyArray.IsValidIndex(i))
            heartsEmptyArray[i]->SetHiddenInGame(!lost);
    }
}