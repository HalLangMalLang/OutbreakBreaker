#include "Library/JMUIFunctionLibrary.h"
#include "Subsystems/JMUISubsystem.h"

UJMUISubsystem* UJMUIFunctionLibrary::GetUISubsystem(const UObject* WorldContextObject)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		return nullptr;
	}

	UGameInstance* GameInstance = World->GetGameInstance();
	if (!GameInstance)
	{
		return nullptr;
	}

	return GameInstance->GetSubsystem<UJMUISubsystem>();
}

UJMWidgetControllerBase* UJMUIFunctionLibrary::GetWidgetController(
	const UObject* WorldContextObject,
	TSubclassOf<UJMWidgetControllerBase> ControllerClass,
	EJMWidgetControllerParamsType ParamsType)
{
	UJMUISubsystem* UISubsystem = GetUISubsystem(WorldContextObject);
	if (!UISubsystem) return nullptr;

	APlayerController* PC = GetPlayerControllerFromContext(WorldContextObject);
	if (!PC) return nullptr;

	return UISubsystem->GetWidgetController(ControllerClass, PC, ParamsType);
}

UJMWidgetControllerBase* UJMUIFunctionLibrary::GetGASWidgetController(
	const UObject* WorldContextObject,
	TSubclassOf<UJMWidgetControllerBase> ControllerClass)
{
	return GetWidgetController(WorldContextObject, ControllerClass, EJMWidgetControllerParamsType::WithGAS);
}

UJMWidgetControllerBase* UJMUIFunctionLibrary::GetSimpleWidgetController(
	const UObject* WorldContextObject,
	TSubclassOf<UJMWidgetControllerBase> ControllerClass)
{
	return GetWidgetController(WorldContextObject, ControllerClass, EJMWidgetControllerParamsType::Simple);
}


UJMUserWidgetBase* UJMUIFunctionLibrary::CreateJMWidget(const UObject* WorldContextObject, TSubclassOf<UJMUserWidgetBase> WidgetClass)
{
	if (APlayerController* PC = GetPlayerControllerFromContext(WorldContextObject))
	{
		if (UJMUISubsystem* JMUISubsystem = GetUISubsystem(PC))
		{
			return JMUISubsystem->JMCreateWidget(WidgetClass);
		}
	}

	return nullptr;
}

APlayerController* UJMUIFunctionLibrary::GetPlayerControllerFromContext(const UObject* WorldContextObject)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}

	// WorldContextObject가 PlayerController인 경우
	if (const APlayerController* PC = Cast<APlayerController>(WorldContextObject))
	{
		return const_cast<APlayerController*>(PC);
	}

	// WorldContextObject가 Pawn인 경우
	if (const APawn* Pawn = Cast<APawn>(WorldContextObject))
	{
		return Cast<APlayerController>(Pawn->GetController());
	}

	// World의 첫 번째 PlayerController 사용
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (World)
	{
		return World->GetFirstPlayerController();
	}

	return nullptr;
}