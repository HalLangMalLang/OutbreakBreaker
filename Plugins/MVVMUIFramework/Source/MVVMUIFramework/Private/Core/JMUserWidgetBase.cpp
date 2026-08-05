#include "Core/JMUserWidgetBase.h" 

void UJMUserWidgetBase::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}