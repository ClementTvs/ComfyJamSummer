// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Customer.generated.h"

UCLASS()
class COMFYJAMSUMMER_API ACustomer : public AActor
{
    GENERATED_BODY()
public:
    ACustomer();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Customer")
    EDrinkType OrderedDrink;

    UPROPERTY(EditAnywhere, Category = "Customer")
    float MaxPatience = 20.f;

    UFUNCTION(BlueprintCallable, Category = "Customer")
    void ReceiveGlass(class AGlass* Glass);

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, Category = "Visual")
    class UPaperSpriteComponent* SpriteComp;

    UPROPERTY(EditAnywhere, Category = "Customer|Sprites")
    class UPaperSprite* NormalSprite;
    UPROPERTY(EditAnywhere, Category = "Customer|Sprites")
    class UPaperSprite* DeadSprite;

    float CurrentPatience;
    bool bServed = false;

    void Serve();
    void Die();
    void Leave();
};