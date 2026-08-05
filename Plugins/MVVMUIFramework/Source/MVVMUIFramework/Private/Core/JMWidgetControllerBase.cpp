#include "Core/JMWidgetControllerBase.h"

void UJMWidgetControllerBase::SetWidgetControllerParams(const FJMWidgetControllerParams& WCParams)
{
	if (bIsInitialized)
	{
		UE_LOG(LogTemp, Warning, TEXT("[JMWidgetController] SetWidgetControllerParams called multiple times. Ignoring."));
		return;
	}

	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	GameState = WCParams.GameState;

	bIsInitialized = true;

	UE_LOG(LogTemp, Log, TEXT("[JMWidgetController] Initialized successfully"));
}

void UJMWidgetControllerBase::BroadcastInitialValues()
{
	// 자식 클래스에서 오버라이드
}

void UJMWidgetControllerBase::BindCallbacksToDependencies()
{
	// 자식 클래스에서 오버라이드
}