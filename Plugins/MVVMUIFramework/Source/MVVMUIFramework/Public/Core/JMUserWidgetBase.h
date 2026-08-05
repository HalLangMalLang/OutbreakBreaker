#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JMUserWidgetBase.generated.h"

/**
 * 사용자 위젯 베이스 클래스
 *
 * 역할:
 * - 위젯 컨트롤러와의 의존성 관리
 * - 클라이언트별 UI 상태 관리(온라인 멀티플레이 기준, 로컬 스플릿스크린 미지원)
 */
UCLASS()
class MVVMUIFRAMEWORK_API UJMUserWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	* 위젯 컨트롤러 주입 (의존성 주입)
	*
	* @param InWidgetController 주입할 위젯 컨트롤러 객체
	*/
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

	// 연결된 WidgetController(ViewModel)
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

protected:
	/**
	 * Controller 설정 완료 시 호출되는 Blueprint 이벤트
	 * Blueprint에서 이 이벤트를 구현하여 델리게이트 바인딩 수행
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
};