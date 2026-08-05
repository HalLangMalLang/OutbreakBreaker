#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types/JMUITypes.h"
#include "JMUIFunctionLibrary.generated.h"

class UJMWidgetControllerBase;
class UJMOverlayWidgetController;
class UJMUISubsystem;
class UJMHUDWidgetController;
class UJMUserWidgetBase;

UCLASS()
class MVVMUIFRAMEWORK_API UJMUIFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "JM|UI", meta = (WorldContext = "WorldContextObject"))
	static UJMUISubsystem* GetUISubsystem(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "JM|UI|WidgetController", meta = (WorldContext = "WorldContextObject"))
	static UJMWidgetControllerBase* GetWidgetController(const UObject* WorldContextObject, TSubclassOf<UJMWidgetControllerBase> ControllerClass, EJMWidgetControllerParamsType ParamsType = EJMWidgetControllerParamsType::WithGAS);

	UFUNCTION(BlueprintCallable, Category = "JM|UI|WidgetController", meta = (WorldContext = "WorldContextObject"))
	static UJMWidgetControllerBase* GetGASWidgetController(const UObject* WorldContextObject, TSubclassOf<UJMWidgetControllerBase> ControllerClass);

	UFUNCTION(BlueprintCallable, Category = "JM|UI|WidgetController", meta = (WorldContext = "WorldContextObject"))
	static UJMWidgetControllerBase* GetSimpleWidgetController(const UObject* WorldContextObject, TSubclassOf<UJMWidgetControllerBase> ControllerClass);

	UFUNCTION(BlueprintCallable, Category = "JM|UI|Utility", meta = (WorldContext = "WorldContextObject"))
	static UJMUserWidgetBase* CreateJMWidget(const UObject* WorldContextObject, TSubclassOf<UJMUserWidgetBase> WidgetClass);

private:
	// WorldContextObject에서 플레이어 컨트롤러 추출
	static APlayerController* GetPlayerControllerFromContext(const UObject* WorldContextObject);
};