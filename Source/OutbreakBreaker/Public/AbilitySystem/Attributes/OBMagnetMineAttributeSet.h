#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/OBWeaponAttributeSetBase.h"
#include "OBMagnetMineAttributeSet.generated.h"

UCLASS()
class OUTBREAKBREAKER_API UOBMagnetMineAttributeSet : public UOBWeaponAttributeSetBase
{
	GENERATED_BODY()

public:
	// =====================================================================
	// [무기 고유 액티브/패시브 스탯 - 마그넷 지뢰 전용]
	// =====================================================================
	// 패시브 계열 - 마그넷 지뢰 폭발까지 걸리는 지연 시간
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Passive")
	FGameplayAttributeData PassiveExplosionDelay;
	ATTRIBUTE_ACCESSORS(UOBMagnetMineAttributeSet, PassiveExplosionDelay);

	// 패시브 계열 - 마그넷 지뢰 폭발 반경 범위
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Passive")
	FGameplayAttributeData PassiveExplosionRadius;
	ATTRIBUTE_ACCESSORS(UOBMagnetMineAttributeSet, PassiveExplosionRadius);

	// 패시브 계열 - 공격 적중 시 적의 이동속도를 깎아내릴 감속 배율 (ex: 0.3 = 30% 감속)
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Passive")
	FGameplayAttributeData PassiveSlowAmount;
	ATTRIBUTE_ACCESSORS(UOBMagnetMineAttributeSet, PassiveSlowAmount);

	// 패시브 계열 - 슬로우 감속 디버프가 적에게 유지될 지속 시간 (초)
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Passive")
	FGameplayAttributeData PassiveSlowTimer;
	ATTRIBUTE_ACCESSORS(UOBMagnetMineAttributeSet, PassiveSlowTimer);

	// 패시브/액티브 공통 추가 반경 (고정 보너스)
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Bonus")
	FGameplayAttributeData BonusExplosionRadius;
	ATTRIBUTE_ACCESSORS(UOBMagnetMineAttributeSet, BonusExplosionRadius);

	// =====================================================================
	// [제어 계열]
	// =====================================================================
	// 현재 무기의 강화 레벨
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Control")
	FGameplayAttributeData WeaponLevel;
	ATTRIBUTE_ACCESSORS(UOBMagnetMineAttributeSet, WeaponLevel);

	// =====================================================================
	// [곱연산 배율 스케일링(Multiplier) 주머니 세트]
	// =====================================================================
	// 무기 전용 고유 공격력 배율 (기본 1.0f -> 증강/시너지 시 곱연산 스케일링)
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Multiplier")
	FGameplayAttributeData WeaponDamageMultiplier;
	ATTRIBUTE_ACCESSORS(UOBMagnetMineAttributeSet, WeaponDamageMultiplier);

	// 무기 전용 고유 공격속도 배율 (기본 1.0f -> 개별 무기 속사포 튜닝선)
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Multiplier")
	FGameplayAttributeData WeaponAttackSpeedMultiplier;
	ATTRIBUTE_ACCESSORS(UOBMagnetMineAttributeSet, WeaponAttackSpeedMultiplier);

	// 무기 전용 고유 공격 범위/사거리 배율 (기본 1.0f -> 거대 장판/초광역 기믹용)
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Multiplier")
	FGameplayAttributeData WeaponRangeMultiplier;
	ATTRIBUTE_ACCESSORS(UOBMagnetMineAttributeSet, WeaponRangeMultiplier);

	// =====================================================================
	// [고정 값 더하기용 보너스(Additive)]
	// =====================================================================
	// 무기 고유 고정 데미지 추가량 (ex - 증강으로 데미지 +5 고정 증가 시)
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Bonus")
	FGameplayAttributeData BonusDamage;
	ATTRIBUTE_ACCESSORS(UOBMagnetMineAttributeSet, BonusDamage);

	// 무기 고유 고정 공격 딜레이 감소량 (초 단위 고정 차감선)
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Bonus")
	FGameplayAttributeData BonusAttackDelay;
	ATTRIBUTE_ACCESSORS(UOBMagnetMineAttributeSet, BonusAttackDelay);

	// 무기 고유 고정 공격 범위/사거리 추가량 (cm 단위 고정 확장선)
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Bonus")
	FGameplayAttributeData BonusRange;
	ATTRIBUTE_ACCESSORS(UOBMagnetMineAttributeSet, BonusRange);

	// =====================================================================
	// [패시브 계열 공통 속성선]
	// =====================================================================
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Passive")
	FGameplayAttributeData PassiveDamage;
	ATTRIBUTE_ACCESSORS(UOBMagnetMineAttributeSet, PassiveDamage);

	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Passive")
	FGameplayAttributeData PassiveAttackDelay;
	ATTRIBUTE_ACCESSORS(UOBMagnetMineAttributeSet, PassiveAttackDelay);

	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Passive")
	FGameplayAttributeData PassiveAttackRange;
	ATTRIBUTE_ACCESSORS(UOBMagnetMineAttributeSet, PassiveAttackRange);

	// =====================================================================
	// [액티브 계열 공통 속성선]
	// =====================================================================
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Selected")
	FGameplayAttributeData SelectedDamage;
	ATTRIBUTE_ACCESSORS(UOBMagnetMineAttributeSet, SelectedDamage);

	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Selected")
	FGameplayAttributeData SelectedAttackDelay;
	ATTRIBUTE_ACCESSORS(UOBMagnetMineAttributeSet, SelectedAttackDelay);

	// =====================================================================
	// [공통 유틸리티 속성]
	// =====================================================================
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Common")
	FGameplayAttributeData CommonAttackSpeed;
	ATTRIBUTE_ACCESSORS(UOBMagnetMineAttributeSet, CommonAttackSpeed);
};