#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Core/JMWidgetControllerBase.h"
#include "Types/JMUITypes.h"
#include "JMUISubsystem.generated.h"

class UJMUserWidgetBase;

UCLASS()
class MVVMUIFRAMEWORK_API UJMUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	// 위젯 컨트롤러 가져오기 - TSubclassOf 방식 
	UFUNCTION(BlueprintCallable, Category = "JMUI|Subsystem")
	UJMWidgetControllerBase* GetWidgetController(
		TSubclassOf<UJMWidgetControllerBase> ControllerClass,
		APlayerController* PC,
		EJMWidgetControllerParamsType ParamsType = EJMWidgetControllerParamsType::WithGAS
	);

	// 위젯 생성
	UFUNCTION(BlueprintCallable, Category = "JMUI|Subsystem")
	UJMUserWidgetBase* JMCreateWidget(TSubclassOf<UJMUserWidgetBase> WidgetClass);

private:
	// Controller 캐시 (클래스별로 하나씩만 생성)
	// ⚠️ 캐시 키가 ControllerClass만 사용 → GameInstance당 클래스별 1개 인스턴스
	// 온라인 멀티플레이/싱글플레이 기준 설계 (GameInstance가 클라이언트별로 분리됨)
	// 로컬 스플릿스크린(1개 GameInstance 안에 여러 PlayerController) 미지원
	UPROPERTY()
	TMap<TSubclassOf<UJMWidgetControllerBase>, FJMCachedController> WidgetControllers;

	// FJMWidgetControllerParams 생성 헬퍼
	FJMWidgetControllerParams MakeWidgetControllerParams(APlayerController* PC);

	FJMWidgetControllerParams MakeSimpleParams(APlayerController* PC);
};