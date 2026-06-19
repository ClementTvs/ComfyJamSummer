#pragma once
#include "CoreMinimal.h"
#include "MoveableSprite.h"
#include "IngredientsTypes.h"
#include "Ingredients.generated.h"

class UPouring;

UCLASS()
class COMFYJAMSUMMER_API AIngredients : public AMoveableSprite
{
    GENERATED_BODY()

public:
    AIngredients();
    virtual void BeginPlay() override;
    const EIngredientsTypes &getIngredientType() const;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere)
    UPouring* pourSpout;

    UPROPERTY(EditAnywhere)
    UPaperSpriteComponent *fillSprite;

    UPROPERTY(EditAnywhere)
    EIngredientsTypes ingredientType;

    UFUNCTION(BlueprintCallable, Category = "Ingredient")
    void OnGrabbed();

    UFUNCTION(BlueprintCallable, Category = "Ingredient")
    void OnReleased();
};