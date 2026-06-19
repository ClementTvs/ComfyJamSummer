#include "Pouring.h"
#include "GameFramework/Actor.h"

UPouring::UPouring()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UPouring::KeepPouring(float receiverSide)
{
    bPouring   = true;
    currentDir = bFixedDirection ? fixedDirection : (receiverSide * directionSign);
    if (const UWorld* w = GetWorld())
        lastBeat = w->GetTimeSeconds();
}

void UPouring::TickComponent(float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    AActor* owner = GetOwner();
    if (!owner) return;

    if (bPouring && GetWorld()->GetTimeSeconds() - lastBeat > leaseSeconds)
        bPouring = false;

    const float target = bPouring ? pourPitch * currentDir : 0.f;
    FRotator rot = owner->GetActorRotation();

    if (!bPouring && FMath::IsNearlyZero(rot.Pitch, 0.05f))
        return;

    rot.Pitch = FMath::FInterpTo(rot.Pitch, target, DeltaTime, interpSpeed);
    owner->SetActorRotation(rot);
}

bool UPouring::IsPouring() const 
{
	return bPouring;
}