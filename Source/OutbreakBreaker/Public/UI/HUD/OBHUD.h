#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "OBHUD.generated.h"

class UJMUserWidgetBase;

UCLASS()
class OUTBREAKBREAKER_API AOBHUD : public AHUD
{
	GENERATED_BODY()

public:
	void InitOBHUD();

private:
	UPROPERTY(EditAnywhere, Category = "HS|UI")
	TSubclassOf<UJMUserWidgetBase> HUDWidgetClass;

	UPROPERTY()
	TObjectPtr<UJMUserWidgetBase> HUDWidget;
};
