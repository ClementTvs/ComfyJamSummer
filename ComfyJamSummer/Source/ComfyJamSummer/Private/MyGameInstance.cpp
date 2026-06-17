#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UMyGameInstance::OnStart()
{
    Super::OnStart();
    if (menuMusic)
    {
        audioComponent = UGameplayStatics::SpawnSound2D(this, menuMusic);
        if (audioComponent)
        {
            audioComponent->SetUISound(true);
            audioComponent->bAutoDestroy = false;
            audioComponent->SetVolumeMultiplier(musicVolume);
        }
    }
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
        audioComponent->FadeOut(1.0f, 0.0f);
    if (music)
    {
        audioComponent = UGameplayStatics::SpawnSound2D(this, music);
        if (audioComponent)
            audioComponent->SetVolumeMultiplier(musicVolume);
    }
}