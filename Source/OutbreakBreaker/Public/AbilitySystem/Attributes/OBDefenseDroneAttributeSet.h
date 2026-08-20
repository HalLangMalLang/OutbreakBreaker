#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/OBWeaponAttributeSetBase.h"
#include "OBDefenseDroneAttributeSet.generated.h"

UCLASS()
class OUTBREAKBREAKER_API UOBDefenseDroneAttributeSet : public UOBWeaponAttributeSetBase
{
	GENERATED_BODY()

public:
	// =====================================================================
	// [무기 고유 액티브/패시브 스탯 - 디펜스 드론 전용]
	// =====================================================================
	// 패시브 계열 - 캐릭터 주변 공전 회전 반경
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Passive")
	FGameplayAttributeData PassiveOrbitRadius;
	ATTRIBUTE_ACCESSORS(UOBDefenseDroneAttributeSet, PassiveOrbitRadius);

	// 패시브 계열 - 투사체를 튕겨내거나 소멸시키는 감시 딜레이
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Passive")
	FGameplayAttributeData PassiveDeflectProjectileDelay;
	ATTRIBUTE_ACCESSORS(UOBDefenseDroneAttributeSet, PassiveDeflectProjectileDelay);

	// 패시브 계열 - 근처에 접근한 적을 강제로 넉백 장벽으로 밀쳐내는 딜레이
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Passive")
	FGameplayAttributeData PassiveDeflectEnemyDelay;
	ATTRIBUTE_ACCESSORS(UOBDefenseDroneAttributeSet, PassiveDeflectEnemyDelay);

	// 패시브 계열 - 좀비를 밀쳐낼 때 적용할 물리적 넉백 힘의 밀도
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Passive")
	FGameplayAttributeData PassivePushForce;
	ATTRIBUTE_ACCESSORS(UOBDefenseDroneAttributeSet, PassivePushForce);

	// 패시브/액티브 공통 추가 반경 (고정 보너스)
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Bonus")
	FGameplayAttributeData BonusOrbitRadius;
	ATTRIBUTE_ACCESSORS(UOBDefenseDroneAttributeSet, BonusOrbitRadius);

	// =====================================================================
	// [제어 계열]
	// =====================================================================
	// 현재 무기의 강화 레벨
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Control")
	FGameplayAttributeData WeaponLevel;
	ATTRIBUTE_ACCESSORS(UOBDefenseDroneAttributeSet, WeaponLevel);

	// =====================================================================
	// [곱연산 배율 스케일링(Multiplier) 주머니 세트]
	// =====================================================================
	// 무기 전용 고유 공격력 배율 (기본 1.0f -> 증강/시너지 시 곱연산 스케일링)
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Multiplier")
	FGameplayAttributeData WeaponDamageMultiplier;
	ATTRIBUTE_ACCESSORS(UOBDefenseDroneAttributeSet, WeaponDamageMultiplier);

	// 무기 전용 고유 공격속도 배율 (기본 1.0f -> 개별 무기 속사포 튜닝선)
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Multiplier")
	FGameplayAttributeData WeaponAttackSpeedMultiplier;
	ATTRIBUTE_ACCESSORS(UOBDefenseDroneAttributeSet, WeaponAttackSpeedMultiplier);

	// 무기 전용 고유 공격 범위/사거리 배율 (기본 1.0f -> 거대 장판/초광역 기믹용)
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Multiplier")
	FGameplayAttributeData WeaponRangeMultiplier;
	ATTRIBUTE_ACCESSORS(UOBDefenseDroneAttributeSet, WeaponRangeMultiplier);

	// =====================================================================
	// [고정 값 더하기용 보너스(Additive)]
	// =====================================================================
	// 무기 고유 고정 데미지 추가량 (ex - 증강으로 데미지 +5 고정 증가 시)
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Bonus")
	FGameplayAttributeData BonusDamage;
	ATTRIBUTE_ACCESSORS(UOBDefenseDroneAttributeSet, BonusDamage);

	// 무기 고유 고정 공격 딜레이 감소량 (초 단위 고정 차감선)
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Bonus")
	FGameplayAttributeData BonusAttackDelay;
	ATTRIBUTE_ACCESSORS(UOBDefenseDroneAttributeSet, BonusAttackDelay);

	// 무기 고유 고정 공격 범위/사거리 추가량 (cm 단위 고정 확장선)
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Bonus")
	FGameplayAttributeData BonusRange;
	ATTRIBUTE_ACCESSORS(UOBDefenseDroneAttributeSet, BonusRange);

	// =====================================================================
	// [패시브 계열 공통 속성선]
	// =====================================================================
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Passive")
	FGameplayAttributeData PassiveDamage;
	ATTRIBUTE_ACCESSORS(UOBDefenseDroneAttributeSet, PassiveDamage);

	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Passive")
	FGameplayAttributeData PassiveAttackDelay;
	ATTRIBUTE_ACCESSORS(UOBDefenseDroneAttributeSet, PassiveAttackDelay);

	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Passive")
	FGameplayAttributeData PassiveAttackRange;
	ATTRIBUTE_ACCESSORS(UOBDefenseDroneAttributeSet, PassiveAttackRange);

	// =====================================================================
	// [액티브 계열 공통 속성선]
	// =====================================================================
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Selected")
	FGameplayAttributeData SelectedDamage;
	ATTRIBUTE_ACCESSORS(UOBDefenseDroneAttributeSet, SelectedDamage);

	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Selected")
	FGameplayAttributeData SelectedAttackDelay;
	ATTRIBUTE_ACCESSORS(UOBDefenseDroneAttributeSet, SelectedAttackDelay);

	// =====================================================================
	// [공통 유틸리티 속성]
	// =====================================================================
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Common")
	FGameplayAttributeData CommonAttackSpeed;
	ATTRIBUTE_ACCESSORS(UOBDefenseDroneAttributeSet, CommonAttackSpeed);
};