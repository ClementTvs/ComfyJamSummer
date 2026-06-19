// BackgroundActor.h
#pragma once
#include "CoreMinimal.h"
#include "PaperSpriteComponent.h"
#include "GameFramework/Actor.h"
#include "BackgroundActor.generated.h"

UCLASS()
class COMFYJAMSUMMER_API ABackgroundActor : public AActor
{
    GENERATED_BODY()

	public:
		ABackgroundActor();

		UPROPERTY(EditAnywhere)
		UPaperSpriteComponent* baseBackground;

		UPROPERTY(EditAnywhere)
		UPaperSpriteComponent* alien1;

		UPROPERTY(EditAnywhere)
		UPaperSpriteComponent* alien2;

		UPROPERTY(EditAnywhere)
		UPaperSpriteComponent* alien3;

		void UpdateBackground(int32 DrinksSold);

	protected:
		virtual void BeginPlay() override;
};