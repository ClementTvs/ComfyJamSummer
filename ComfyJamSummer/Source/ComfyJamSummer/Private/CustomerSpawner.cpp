// Fill out your copyright notice in the Description page of Project Settings.
#include "CustomerSpawner.h"
#include "Customer.h"
#include "TimerManager.h"

ACustomerSpawner::ACustomerSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACustomerSpawner::BeginPlay()
{
	Super::BeginPlay();

	occupants.SetNum(spawnPoints.Num());
	respawnTimers.SetNum(spawnPoints.Num());

	GetWorldTimerManager().SetTimer(spawnCheckTimer, this,
		&ACustomerSpawner::CheckAndSpawn, checkInterval, true);

	TArray<int32> order;
	for (int32 i = 0; i < spawnPoints.Num(); ++i)
		order.Add(i);

	for (int32 i = order.Num() - 1; i > 0; --i)
	{
		const int32 j = FMath::RandRange(0, i);
		order.Swap(i, j);
	}

	for (int32 k = 0; k < order.Num(); ++k)
	{
		const int32 slot = order[k];
		const float delay = initialSpawnInterval * (k + 1);

		FTimerDelegate del = FTimerDelegate::CreateUObject(
			this, &ACustomerSpawner::SpawnAtSlot, slot);

		GetWorldTimerManager().SetTimer(respawnTimers[slot], del, delay, false);
	}
}

void ACustomerSpawner::CheckAndSpawn()
{
	if (customerClasses.Num() == 0)
		return;

	for (int32 i = 0; i < spawnPoints.Num(); ++i)
	{
		if (IsValid(occupants[i]))
			continue;

		if (GetWorldTimerManager().IsTimerActive(respawnTimers[i]))
			continue;

		if (!spawnPoints[i])
			continue;

		FTimerDelegate del = FTimerDelegate::CreateUObject(
			this, &ACustomerSpawner::SpawnAtSlot, i);

		GetWorldTimerManager().SetTimer(respawnTimers[i], del, spawnDelay, false);
	}
}

void ACustomerSpawner::SpawnAtSlot(int32 index)
{
	if (!spawnPoints.IsValidIndex(index) || !spawnPoints[index] || customerClasses.Num() == 0)
		return;

	const int32 pick = FMath::RandRange(0, customerClasses.Num() - 1);
	TSubclassOf<ACustomer> chosenClass = customerClasses[pick];
	if (!chosenClass)
		return;

	FTransform spawnTransform = spawnPoints[index]->GetActorTransform();

	ACustomer* newCustomer = GetWorld()->SpawnActor<ACustomer>(chosenClass, spawnTransform);
	occupants[index] = newCustomer;
}