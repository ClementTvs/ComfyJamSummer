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

        UPROPERTY(EditAnywhere, Category = "Audio")
        USoundBase* gameMusic;

        UPROPERTY(EditAnywhere, Category = "Audio")
        USoundBase* goodEndingMusic;

        UPROPERTY(EditAnywhere, Category = "Audio")
        USoundBase* badEndingMusic;


        UPROPERTY()
        UAudioComponent* audioComponent = nullptr;

		UAudioComponent* SpawnSFX(USoundBase* sound, float volumeScale = 1.0f);

		virtual void OnStart() override;

        float GetMusicVolume() const;

        void SetVolume(float volume);
        void PlayMusic(USoundBase* music);
};