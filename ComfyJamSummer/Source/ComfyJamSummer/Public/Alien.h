#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Alien.generated.h"

UENUM(BlueprintType)
enum class EAlienState : uint8
{
    Idle        UMETA(DisplayName = "Idle"),
    LookAway       UMETA(DisplayName = "LookAway")
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

protected:
    virtual void BeginPlay() override;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visual")
    class UPaperFlipbookComponent* FlipbookComp;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visual")
    class UPaperZDAnimationComponent* AnimComp;

public:
    virtual void Tick(float DeltaTime) override;
};