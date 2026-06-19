#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Pouring.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COMFYJAMSUMMER_API UPouring : public UActorComponent
{
    GENERATED_BODY()
public:
    UPouring();

    UPROPERTY(EditAnywhere, Category="Pour") 
	float pourPitch = 70.f;
    UPROPERTY(EditAnywhere, Category="Pour")
	float interpSpeed = 6.f;
    UPROPERTY(EditAnywhere, Category="Pour")
	float leaseSeconds = 0.08f;
    UPROPERTY(EditAnywhere, Category="Pour")
	bool  bFixedDirection = false;
    UPROPERTY(EditAnywhere, Category="Pour")
	float fixedDirection = 1.f;
	UPROPERTY(EditAnywhere, Category = "Pour")
	float directionSign = 1.f;

    void KeepPouring(float receiverSide);
    bool IsPouring() const;

    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
        FActorComponentTickFunction* ThisTickFunction) override;

private:
    bool  bPouring = false;
    float currentDir = -1.f;
    float lastBeat = -1000.f;
};