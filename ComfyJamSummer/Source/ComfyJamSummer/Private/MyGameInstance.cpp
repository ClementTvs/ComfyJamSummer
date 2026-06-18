#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UMyGameInstance::OnStart()
{
    Super::OnStart();
    if (audioComponent)
    {
        audioComponent->SetUISound(true);
        audioComponent->bAutoDestroy = false;
        audioComponent->SetVolumeMultiplier(musicVolume);
    }
}

float UMyGameInstance::GetMusicVolume() const
{
    return musicVolume;
}

void UMyGameInstance::SetVolume(float volume)
{
    musicVolume = volume;
    if (audioComponent && audioComponent->IsValidLowLevel())
        audioComponent->SetVolumeMultiplier(volume);
}

void UMyGameInstance::PlayMusic(USoundBase* music)
{
    if (audioComponent && audioComponent->IsValidLowLevel())
    {
        audioComponent->Stop();
        audioComponent = nullptr;
    }
    if (music)
    {
        audioComponent = UGameplayStatics::SpawnSound2D(this, music);
        if (audioComponent)
            audioComponent->SetVolumeMultiplier(musicVolume);
    }
}

UAudioComponent* UMyGameInstance::SpawnSFX(USoundBase* sound, float volumeScale)
{
    if (!sound)
        return nullptr;
    return UGameplayStatics::SpawnSound2D(this, sound, musicVolume * volumeScale);
}