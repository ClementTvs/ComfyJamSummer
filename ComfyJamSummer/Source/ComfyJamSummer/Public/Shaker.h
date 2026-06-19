#pragma once
#include "CoreMinimal.h"
#include "MoveableSprite.h"
#include "Drinks.h"
#include "Components/ProgressBar.h"
#include "Components/WidgetComponent.h"
#include "IngredientsTypes.h"
#include "MyPlayerController.h"
#include "Ingredients.h"
#include "MyGameInstance.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "Shaker.generated.h"

class AIngredients;
class UPouring;

UCLASS()
class COMFYJAMSUMMER_API AShaker : public AMoveableSprite
{
    GENERATED_BODY()

private:
    TArray<EIngredientsTypes> currentIngredients;
    EDrinks drink = EDrinks::noDrink;
    AIngredients *pendingIngredient = nullptr;
    UPouring* pendingIngredientSpout = nullptr;
    FTimerHandle IngredientTimer;
    float timerDuration;
    float ShakePower = 0.f;
    FVector2D LastMousePos;
    AMyPlayerController* pc;
    bool canShake = false;

    UFUNCTION()
    void OnIngredientOverlap(UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnIngredientEndOverlap(UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    void ValidateIngredient();
    void makeDrink();
    bool ContainsRecipe(const TArray<EIngredientsTypes>& recipe);
    void CancelIngredientPour();

    void StartShakerSound();
    void StopShakerSound();

public:
    AShaker();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    void resetDrink();
    EDrinks getDrink() const;

    UPROPERTY(VisibleAnywhere)
    UPouring* pourSpout;

    UPROPERTY(VisibleAnywhere)
    UPaperSpriteComponent* shakerOpenSprite;

    UPROPERTY(VisibleAnywhere)
    UWidgetComponent* timerWidgetInstance;

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> timerWidgetClass;

    UPROPERTY(VisibleAnywhere)
    UBoxComponent *fillHitBox;

    UPROPERTY(EditAnywhere, Category = "Audio")
    USoundBase* shakerSound;

    UPROPERTY()
    UAudioComponent* shakerAudio = nullptr;

	UPROPERTY(EditAnywhere, Category = "Pour")
	float fillMaxDistance = 100.f;
};