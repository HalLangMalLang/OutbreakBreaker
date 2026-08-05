#pragma once

#include "CoreMinimal.h"
#include "Core/JMWidgetControllerBase.h"
#include "JMUITypes.generated.h"

/**
 * 위젯 컨트롤러 파라미터 타입
 * - Simple: PC, PS만 사용 (GAS 불필요)
 * - WithGAS: PC, PS, ASC, AS 사용 (GAS 필수)
 */
UENUM(BlueprintType)
enum class EJMWidgetControllerParamsType : uint8
{
	Simple    UMETA(DisplayName = "Simple (PC/PS Only)"),
	WithGAS   UMETA(DisplayName = "With GAS (ASC/AS Required)")
};


USTRUCT()
struct FJMCachedController
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UJMWidgetControllerBase> Controller = nullptr;

	UPROPERTY()
	EJMWidgetControllerParamsType ParamsType = EJMWidgetControllerParamsType::Simple;
};