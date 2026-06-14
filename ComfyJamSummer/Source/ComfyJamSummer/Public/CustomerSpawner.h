#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomerSpawner.generated.h"
class ACustomer;
UCLASS()
class COMFYJAMSUMMER_API ACustomerSpawner : public AActor
{
    GENERATED_BODY()
public:
    ACustomerSpawner();
protected:
    virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, Category = "Spawn")
	TArray<TSubclassOf<ACustomer>> customerClasses;
    UPROPERTY(EditAnywhere, Category = "Spawn")
    TArray<AActor*> spawnPoints;
    UPROPERTY(EditAnywhere, Category = "Spawn")
    float checkInterval = 1.f;
    UPROPERTY(EditAnywhere, Category = "Spawn")
    float spawnDelay = 15.f;
	UPROPERTY(EditAnywhere, Category = "Spawn")
	float initialSpawnInterval = 3.f;
private:
    UPROPERTY()
    TArray<ACustomer*> occupants;
    TArray<FTimerHandle> respawnTimers;
    FTimerHandle spawnCheckTimer;
    void CheckAndSpawn();
    void SpawnAtSlot(int32 index);
};