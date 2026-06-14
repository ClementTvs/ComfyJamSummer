// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Drinks.h"
#include "Customer.generated.h"
class AGlass;

UENUM()
enum class ECustomerState : uint8
{
    Neutral,
    Annoyed,
    Angry,
    Served
};

UCLASS()
class COMFYJAMSUMMER_API ACustomer : public AActor
{
    GENERATED_BODY()

private:
    bool isOverCustomer = false;

    UFUNCTION()
    void OnGlassLeaveCustomer(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex);

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnGlassOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

public:
    ACustomer();

    void SellDrink();
    bool getIsOverCustomer() const;

    UPROPERTY(VisibleAnywhere)
    class UPaperSpriteComponent* spriteComp;

    UPROPERTY(VisibleAnywhere)
    class UBoxComponent* serveHitBox;

    UPROPERTY(VisibleAnywhere)
    class UPaperSpriteComponent* orderComp;

    UPROPERTY(EditAnywhere, Category = "Order")
    TMap<EDrinks, class UPaperSprite*> orderSprites;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Order")
    EDrinks desiredDrink = EDrinks::noDrink;

    UPROPERTY(EditAnywhere, Category = "Sprites|Waiting")
    class UPaperSprite* neutralSprite;
    UPROPERTY(EditAnywhere, Category = "Sprites|Waiting")
    class UPaperSprite* annoyedSprite;
    UPROPERTY(EditAnywhere, Category = "Sprites|Waiting")
    class UPaperSprite* angrySprite;

    UPROPERTY(EditAnywhere, Category = "Sprites|Received")
    class UPaperSprite* receivedNeutralSprite;
    UPROPERTY(EditAnywhere, Category = "Sprites|Received")
    class UPaperSprite* receivedAnnoyedSprite;
    UPROPERTY(EditAnywhere, Category = "Sprites|Received")
    class UPaperSprite* receivedAngrySprite;

    UPROPERTY(EditAnywhere, Category = "Patience") float patience = 100.f;
    UPROPERTY(EditAnywhere, Category = "Patience") float patienceLossPerStep = 10.f;
    UPROPERTY(EditAnywhere, Category = "Patience") float stepInterval = 1.f;
    UPROPERTY(EditAnywhere, Category = "Patience") float annoyedThreshold = 60.f;
    UPROPERTY(EditAnywhere, Category = "Patience") float angryThreshold = 30.f;

	UPROPERTY(EditAnywhere, Category = "Glass")
		TSubclassOf<AGlass> glassClass;

	UPROPERTY(EditAnywhere, Category = "Glass")
	FVector spawnLocation = FVector::ZeroVector;

    void ReceiveDrink();
    ECustomerState GetState() const { return currentState; }

private:
    ECustomerState currentState = ECustomerState::Neutral;
    FTimerHandle patienceTimer;
	FTimerHandle leaveTimer;

	void StartLeaveTimer();
	void Leave();
	void ReceivedWrongDrink();
    void PickRandomOrder();
    void DecreasePatience();
    void UpdateState();
    void ApplyStateSprite();
};