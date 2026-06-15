#include "MainMenu.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainMenu::NativeConstruct()
{
    Super::NativeConstruct();

    if (Button_Play)
    {
        Button_Play->OnClicked.AddDynamic(this, &UMainMenu::OnPlayClicked);
    }

    if (Button_Quit)
    {
        Button_Quit->OnClicked.AddDynamic(this, &UMainMenu::OnQuitClicked);
    }
}


void UMainMenu::OnPlayClicked()
{
    UGameplayStatics::OpenLevel(this, "MainGame");
}


void UMainMenu::OnQuitClicked()
{
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}