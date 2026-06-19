#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyGameInstance.h"
#include "Alien.generated.h"

UENUM(BlueprintType)
enum class EAlienState : uint8
{
    Idle        UMETA(DisplayName = "Idle"),
    LookAway    	UMETA(DisplayName = "LookAway")
};

UCLASS()
class COMFYJAMSUMMER_API AAlien : public AActor
{
    GENERATED_BODY()

public:
    AAlien();
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Alien")
    EAlienState State = EAlienState::Idle;

	UFUNCTION(BlueprintCallable, Category = "Alien")
	void SetAlienState(EAlienState NewState);

	UFUNCTION(BlueprintCallable, Category = "Alien")
	bool isWatching() const;

	UFUNCTION(BlueprintCallable, Category = "Alien")
	void OnGasolineGrab();

protected:
    virtual void BeginPlay() override;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visual")
    class UPaperFlipbookComponent* FlipbookComp;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visual")
    class UPaperZDAnimationComponent* AnimComp;
	UPROPERTY(EditAnywhere, Category = "Alien|Timing")
	float WatchTimeMin = 3.f;
	UPROPERTY(EditAnywhere, Category = "Alien|Timing")
	float WatchTimeMax = 6.f;
	UPROPERTY(EditAnywhere, Category = "Alien|Timing")
	float LookAwayTimeMin = 8.f;
	UPROPERTY(EditAnywhere, Category = "Alien|Timing")
	float LookAwayTimeMax = 11.f;
	UPROPERTY(EditAnywhere, Category = "Alien|Audio")
	USoundBase* turnWarningSound;
	UPROPERTY(EditAnywhere, Category = "Alien|Timing")
	float warningLeadTime = 1.f;

	FTimerHandle WarningTimer;
	FTimerHandle StateTimer;

	void StartWatching();
	void StartLookingAway();
	void OnPlayerCaught();
	void PlayTurnWarning();

public:
	UPROPERTY(BlueprintReadWrite, Category = "Alien")
	bool bGasolineInUse = false;
	
    virtual void Tick(float DeltaTime) override;
};