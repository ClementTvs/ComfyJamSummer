
#include "MusicTrigger.h"
#include "MyGameInstance.h"


void AMusicTrigger::BeginPlay()
{
	Super::BeginPlay();
	if (UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance()))
	{
		GI->PlayMusic(gameMusic);
	}
}


