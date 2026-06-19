#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperSpriteComponent.h"
#include "PlayerHealth.generated.h"

class ABackgroundActor;

UENUM()
enum class EDeathCause : uint8
{
    None,
    Arrest,
    Fired
};

UCLASS()
class COMFYJAMSUMMER_API APlayerHealth : public AActor
{
    GENERATED_BODY()

private:
    int lives = 3;
    int arrestCount = 0;
    int firedCount = 0;
    EDeathCause lockedEnding = EDeathCause::None;

    void UpdateHearts();
    
    void firedEnd();
    void policeEnd();


public:

	UPROPERTY()
	ABackgroundActor* BackgroundActorRef;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> policeEndingScreen;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> firedEndingScreen;
    
    APlayerHealth();
    void LoseLife(EDeathCause cause);

    UPROPERTY(EditAnywhere, Category = "Health")
    int strikesToLose = 2;

    UPROPERTY(VisibleAnywhere)
    TArray<UPaperSpriteComponent*> heartsFullArray;
    UPROPERTY(VisibleAnywhere)
    TArray<UPaperSpriteComponent*> heartsEmptyArray;

    UPROPERTY(VisibleAnywhere)
    USceneComponent *root;
};