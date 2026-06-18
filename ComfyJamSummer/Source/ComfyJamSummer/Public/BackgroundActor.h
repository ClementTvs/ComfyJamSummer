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

		UPROPERTY(VisibleAnywhere)
		UPaperSpriteComponent* baseBackground;

		UPROPERTY(VisibleAnywhere)
		UPaperSpriteComponent* alien1;

		UPROPERTY(VisibleAnywhere)
		UPaperSpriteComponent* alien2;

		UPROPERTY(VisibleAnywhere)
		UPaperSpriteComponent* alien3;

		void UpdateBackground(int32 DrinksSold);

	protected:
		virtual void BeginPlay() override;
};