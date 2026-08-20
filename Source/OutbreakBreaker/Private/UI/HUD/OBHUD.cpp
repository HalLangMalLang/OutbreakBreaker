#include "UI/HUD/OBHUD.h"
#include "Subsystems/JMUISubsystem.h"
#include "UI/WidgetController/OBHUDWidgetController.h"
#include "UI/WidgetController/OBOverlayWidgetController.h"
#include "Core/JMUserWidgetBase.h"

void AOBHUD::InitOBHUD()
{
	APlayerController* PC = GetOwningPlayerController();
	if (!PC)
	{
		return;
	}

	UOBHUDWidgetController* OBHUDWidgetController = UOBHUDWidgetController::Get(PC);
	if (!OBHUDWidgetController)
	{
		return;
	}

	UOBOverlayWidgetController* OBOverlayWidgetController = UOBOverlayWidgetController::Get(PC);
	if (!OBOverlayWidgetController)
	{
		return;
	}

	HUDWidget = GetGameInstance()->GetSubsystem<UJMUISubsystem>()->JMCreateWidget(HUDWidgetClass);
	if (!HUDWidget)
	{
		return;
	}

	HUDWidget->SetWidgetController(OBHUDWidgetController);
	HUDWidget->AddToViewport();
}

void AOBHUD::ShowVictoryScreen()
{

}

void AOBHUD::ShowGameOverScreen()
{

}
