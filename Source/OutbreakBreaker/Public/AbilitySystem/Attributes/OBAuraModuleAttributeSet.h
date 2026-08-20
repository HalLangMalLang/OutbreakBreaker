#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/OBWeaponAttributeSetBase.h"
#include "OBAuraModuleAttributeSet.generated.h"

UCLASS()
class OUTBREAKBREAKER_API UOBAuraModuleAttributeSet : public UOBWeaponAttributeSetBase
{
	GENERATED_BODY()

public:
	// =====================================================================
	// [무기 고유 액티브/패시브 스탯 - 오라 모듈 전용]
	// =====================================================================
	// 패시브 계열 - 오라 장판 적을 둔화시키는 슬로우 영역의 총 반경 크기
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Passive")
	FGameplayAttributeData PassiveSlowAreaRadius;
	ATTRIBUTE_ACCESSORS(UOBAuraModuleAttributeSet, PassiveSlowAreaRadius);

	// 패시브 계열 - 오라 장판 발동 주기 딜레이 
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Passive")
	FGameplayAttributeData PassiveSlowAreaDelay;
	ATTRIBUTE_ACCESSORS(UOBAuraModuleAttributeSet, PassiveSlowAreaDelay);

	// 패시브 계열 - 오라 장판의 field 지속 시간 (초)
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Passive")
	FGameplayAttributeData PassiveSlowAreaTimer;
	ATTRIBUTE_ACCESSORS(UOBAuraModuleAttributeSet, PassiveSlowAreaTimer);

	// 패시브 계열 - 오라 장판 영역을 밟은 적의 슬로우 감속 배율
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Passive")
	FGameplayAttributeData PassiveSlowAreaAmount;
	ATTRIBUTE_ACCESSORS(UOBAuraModuleAttributeSet, PassiveSlowAreaAmount);

	// 패시브/액티브 공통 추가 반경 (고정 보너스)
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Bonus")
	FGameplayAttributeData BonusSlowAreaRadius;
	ATTRIBUTE_ACCESSORS(UOBAuraModuleAttributeSet, BonusSlowAreaRadius);

	// =====================================================================
	// [제어 계열]
	// =====================================================================
	// 현재 무기의 강화 레벨
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Control")
	FGameplayAttributeData WeaponLevel;
	ATTRIBUTE_ACCESSORS(UOBAuraModuleAttributeSet, WeaponLevel);

	// =====================================================================
	// [곱연산 배율 스케일링(Multiplier) 주머니 세트]
	// =====================================================================
	// 무기 전용 고유 공격력 배율 (기본 1.0f -> 증강/시너지 시 곱연산 스케일링)
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Multiplier")
	FGameplayAttributeData WeaponDamageMultiplier;
	ATTRIBUTE_ACCESSORS(UOBAuraModuleAttributeSet, WeaponDamageMultiplier);

	// 무기 전용 고유 공격속도 배율 (기본 1.0f -> 개별 무기 속사포 튜닝선)
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Multiplier")
	FGameplayAttributeData WeaponAttackSpeedMultiplier;
	ATTRIBUTE_ACCESSORS(UOBAuraModuleAttributeSet, WeaponAttackSpeedMultiplier);

	// 무기 전용 고유 공격 범위/사거리 배율 (기본 1.0f -> 거대 장판/초광역 기믹용)
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Multiplier")
	FGameplayAttributeData WeaponRangeMultiplier;
	ATTRIBUTE_ACCESSORS(UOBAuraModuleAttributeSet, WeaponRangeMultiplier);

	// =====================================================================
	// [고정 값 더하기용 보너스(Additive)]
	// =====================================================================
	// 무기 고유 고정 데미지 추가량 (ex - 증강으로 데미지 +5 고정 증가 시)
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Bonus")
	FGameplayAttributeData BonusDamage;
	ATTRIBUTE_ACCESSORS(UOBAuraModuleAttributeSet, BonusDamage);

	// 무기 고유 고정 공격 딜레이 감소량 (초 단위 고정 차감선)
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Bonus")
	FGameplayAttributeData BonusAttackDelay;
	ATTRIBUTE_ACCESSORS(UOBAuraModuleAttributeSet, BonusAttackDelay);

	// 무기 고유 고정 공격 범위/사거리 추가량 (cm 단위 고정 확장선)
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Bonus")
	FGameplayAttributeData BonusRange;
	ATTRIBUTE_ACCESSORS(UOBAuraModuleAttributeSet, BonusRange);

	// =====================================================================
	// [패시브 계열 공통 속성선]
	// =====================================================================
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Passive")
	FGameplayAttributeData PassiveDamage;
	ATTRIBUTE_ACCESSORS(UOBAuraModuleAttributeSet, PassiveDamage);

	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Passive")
	FGameplayAttributeData PassiveAttackDelay;
	ATTRIBUTE_ACCESSORS(UOBAuraModuleAttributeSet, PassiveAttackDelay);

	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Passive")
	FGameplayAttributeData PassiveAttackRange;
	ATTRIBUTE_ACCESSORS(UOBAuraModuleAttributeSet, PassiveAttackRange);

	// =====================================================================
	// [액티브 계열 공통 속성선]
	// =====================================================================
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Selected")
	FGameplayAttributeData SelectedDamage;
	ATTRIBUTE_ACCESSORS(UOBAuraModuleAttributeSet, SelectedDamage);

	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Selected")
	FGameplayAttributeData SelectedAttackDelay;
	ATTRIBUTE_ACCESSORS(UOBAuraModuleAttributeSet, SelectedAttackDelay);

	// =====================================================================
	// [공통 유틸리티 속성]
	// =====================================================================
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Common")
	FGameplayAttributeData CommonAttackSpeed;
	ATTRIBUTE_ACCESSORS(UOBAuraModuleAttributeSet, CommonAttackSpeed);
};