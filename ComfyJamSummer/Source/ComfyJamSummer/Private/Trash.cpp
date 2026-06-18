// Fill out your copyright notice in the Description page of Project Settings.


#include "Trash.h"

ATrash::ATrash()
{
    hitBox->OnComponentBeginOverlap.AddDynamic(this, &ATrash::OnGlassOverlap);
    UE_LOG(LogTemp, Warning, TEXT("SIU"));

}

bool ATrash::getIsOverTrash() const
{
    return isOverTrash;
}

void ATrash::Throw()
{
    AGlass* glass = Cast<AGlass>(UGameplayStatics::GetActorOfClass(GetWorld(), AGlass::StaticClass()));
    UE_LOG(LogTemp, Warning, TEXT("Destroy"));
    glass->Destroy();
    GetWorld()->SpawnActor<AGlass>(glassClass, spawnLocation, FRotator::ZeroRotator);
    isOverTrash = false;
} 

void ATrash::OnGlassOverlap(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
    if (OtherComp->GetName() != TEXT("HitBox"))
        return;
    if (Cast<AGlass>(OtherActor))
        isOverTrash = true;
}

void ATrash::OnGlassLeave(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (Cast<AGlass>(OtherActor))
    {
        isOverTrash = false;
    }
}