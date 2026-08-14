#pragma once

#include "CoreMinimal.h"
#include "Core/JMWidgetControllerBase.h"
#include "Library/JMUIFunctionLibrary.h"
#include "OBOverlayWidgetController.generated.h"

UCLASS(NotBlueprintable, BlueprintType)
class OUTBREAKBREAKER_API UOBOverlayWidgetController : public UJMWidgetControllerBase
{
	GENERATED_BODY()

public:
	JM_WIDGET_CONTROLLER_GETTER(UOBOverlayWidgetController, EJMWidgetControllerParamsType::WithGAS)

	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
};
