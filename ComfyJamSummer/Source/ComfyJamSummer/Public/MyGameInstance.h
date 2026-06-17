// MyGameInstance.h

#pragma once
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "MyGameInstance.generated.h"

UCLASS()
class COMFYJAMSUMMER_API UMyGameInstance : public UGameInstance
{
    GENERATED_BODY()
	private:
		float musicVolume = 0.5f;

    public:

        UPROPERTY(EditAnywhere, Category = "Audio")
        USoundBase* menuMusic;

        UPROPERTY()
        UAudioComponent* audioComponent = nullptr;

		virtual void OnStart() override;

        void SetVolume(float volume);
        void PlayMusic(USoundBase* music);
};