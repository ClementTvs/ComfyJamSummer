#pragma once
#include "CoreMinimal.h"
#include "MoveableSprite.h"
#include "IngredientsTypes.h"
#include "Ingredients.h"
#include "Drinks.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/WidgetComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "MyGameInstance.h"
#include "BlenderTop.generated.h"

class UPouring;

UCLASS()
class COMFYJAMSUMMER_API ABlenderTop : public AMoveableSprite
{
    GENERATED_BODY()

private:
    UPROPERTY()
    TArray<EIngredientsTypes> currentIngredients;
    AIngredients *pendingIngredient = nullptr;
    UPouring* pendingIngredientSpout = nullptr;
    FTimerHandle IngredientTimer;
    EDrinks drink = EDrinks::noDrink;

    UFUNCTION()
    void OnIngredientOverlap(UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnIngredientEndOverlap(UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

		
	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundBase* pourSound;
		
	UPROPERTY()
	UAudioComponent* pourAudio = nullptr;
		
		
		void ValidateIngredient();
	void CancelIngredientPour();
	void StartPourSound();
	void StopPourSound();

public:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    void setDrink(EDrinks newDrink);
    EDrinks getDrink() const;
    void resetDrink();

    ABlenderTop();

    UPROPERTY(VisibleAnywhere)
    UPouring* pourSpout;

    UPROPERTY(VisibleAnywhere)
    UBoxComponent *fillHitBox;

    UPROPERTY(VisibleAnywhere)
    UWidgetComponent* timerWidgetInstance;

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> timerWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Pour")
	float fillMaxDistance = 100.f;
    
	float timerDuration;

    const TArray<EIngredientsTypes>& getCurrentIngredients() const;
    void clearCurrentIngredients();
};