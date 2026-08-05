#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AbilitySystemComponent.h"               
#include "JMWidgetControllerBase.generated.h"

// ──────────────────────────────────────────────────────────
// 자식 WidgetController 클래스 선언용 편의 매크로
// 사용: 클래스 본문 안에서 JM_WIDGET_CONTROLLER_GETTER(UMyController, EJMWidgetControllerParamsType::WithGAS)
// C++ 전용 (UFUNCTION 자동 생성 불가 - Blueprint 노출은 각 클래스에서 개별 UFUNCTION 추가)
// ──────────────────────────────────────────────────────────
#define JM_WIDGET_CONTROLLER_GETTER(ControllerClass, ParamsType) \
	static ControllerClass* Get(const UObject* WorldContextObject) \
	{ \
		return Cast<ControllerClass>( \
			UJMUIFunctionLibrary::GetWidgetController(WorldContextObject, ControllerClass::StaticClass(), ParamsType) \
		); \
	}

/**
 * 위젯 컨트롤러 초기화에 필요한 파라미터
 * 플레이어 컨텍스트 + GAS 컴포넌트 + 게임 상태
 */
USTRUCT(BlueprintType)
struct FJMWidgetControllerParams
{
	GENERATED_BODY()

	FJMWidgetControllerParams() {}

	FJMWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, AGameStateBase* GS)
		: PlayerController(PC), PlayerState(PS), AbilitySystemComponent(ASC), GameState(GS) {
	}

	// ──────────────────────────────────────────────────────────
	// 플레이어 관련
	// ──────────────────────────────────────────────────────────
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;

	// ──────────────────────────────────────────────────────────
	// GAS 관련
	// ──────────────────────────────────────────────────────────
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	// ──────────────────────────────────────────────────────────
	// 게임 전역 상태
	// ──────────────────────────────────────────────────────────
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AGameStateBase> GameState = nullptr;

	// ──────────────────────────────────────────────────────────
	// 유효성 검사
	// ──────────────────────────────────────────────────────────
	bool IsValid() const
	{
		return PlayerController != nullptr
			&& PlayerState != nullptr
			&& AbilitySystemComponent != nullptr
			&& GameState != nullptr;
	}

	bool IsValidSimple() const
	{
		return PlayerController != nullptr
			&& PlayerState != nullptr;
	}
};

/**
 * 위젯 컨트롤러 베이스 클래스
 *
 * 역할:
 * - UI와 게임 로직 사이의 중재자(MVVM의 ViewModel)
 * - GAS 데이터 변경 감지 및 UI 업데이트
 * - 클라이언트별 UI 상태 관리 (온라인 멀티플레이/싱글플레이 기준)
 * 
 *   ※ 로컬 스플릿스크린(1개 GameInstance 내 여러 PlayerController)은 미지원.
 *     지원하려면 UJMUISubsystem의 Controller 캐시 키를 (ControllerClass, PlayerController)
 *     조합으로 바꿔야 함.
 *
 * 사용 흐름:
 * 1. SetWidgetControllerParams() - 컨텍스트 설정
 * 2. BindCallbacksToDependencies() - 데이터 변경 감지 등록
 * 3. BroadcastInitialValues() - 초기 UI 데이터 전송
 */
UCLASS()
class MVVMUIFRAMEWORK_API UJMWidgetControllerBase : public UObject
{
	GENERATED_BODY()

public:
	// ──────────────────────────────────────────────────────────
	// 초기화
	// ──────────────────────────────────────────────────────────

	/**
	 * 컨트롤러 초기화(필수 파라미터 설정)
	 * 한 번만 호출되어야 하며, 중복 호출 시 경고 출력
	 */
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FJMWidgetControllerParams& WCParams);

	/**
	 * 초기화 완료 여부 확인
	 */
	UFUNCTION(BlueprintPure, Category = "WidgetController")
	bool IsInitialized() const { return bIsInitialized; }

	// ──────────────────────────────────────────────────────────
	// 자식 클래스 구현 필요
	// ──────────────────────────────────────────────────────────

	/**
	 * UI 초기 데이터 브로드캐스트
	 * 자식 클래스에서 오버라이드하여 델리게이트 발행
	 */
	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues();

	/**
	 * 데이터 변경 감지 바인딩
	 * 자식 클래스에서 구현 (예: Attribute 변경 콜백 등록)
	 */
	virtual void BindCallbacksToDependencies();

protected:
	// ──────────────────────────────────────────────────────────
	// 게임 컨텍스트 (자식 클래스에서 사용)
	// ──────────────────────────────────────────────────────────

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<AGameStateBase> GameState;

private:
	// 초기화 완료 플래그(중복 초기화 방지)
	bool bIsInitialized = false;
};