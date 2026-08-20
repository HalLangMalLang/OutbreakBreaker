#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/OBWeaponAttributeSetBase.h"
#include "OBPlasmaRifleAttributeSet.generated.h"

UCLASS()
class OUTBREAKBREAKER_API UOBPlasmaRifleAttributeSet : public UOBWeaponAttributeSetBase
{
	GENERATED_BODY()

public:
	// =====================================================================
	// [무기 고유 액티브 스탯 - 플라즈마 라이플 전용]
	// =====================================================================
	// 액티브 계열 - 플라즈마 라이플 전용 관통 횟수
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Selected")
	FGameplayAttributeData SelectedPenetrationCount;
	ATTRIBUTE_ACCESSORS(UOBPlasmaRifleAttributeSet, SelectedPenetrationCount);

	// 고정 보너스 계열 - 플라즈마 라이플 전용 추가 관통 횟수
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Bonus")
	FGameplayAttributeData BonusPenetrationCount;
	ATTRIBUTE_ACCESSORS(UOBPlasmaRifleAttributeSet, BonusPenetrationCount);

	// =====================================================================
	// [제어 계열]
	// =====================================================================
	// 현재 무기의 강화 레벨
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Control")
	FGameplayAttributeData WeaponLevel;
	ATTRIBUTE_ACCESSORS(UOBPlasmaRifleAttributeSet, WeaponLevel);

	// =====================================================================
	// [곱연산 배율 스케일링(Multiplier) 주머니 세트]
	// =====================================================================
	// 무기 전용 고유 공격력 배율 (기본 1.0f -> 증강/시너지 시 곱연산 스케일링)
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Multiplier")
	FGameplayAttributeData WeaponDamageMultiplier;
	ATTRIBUTE_ACCESSORS(UOBPlasmaRifleAttributeSet, WeaponDamageMultiplier);

	// 무기 전용 고유 공격속도 배율 (기본 1.0f -> 개별 무기 속사포 튜닝선)
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Multiplier")
	FGameplayAttributeData WeaponAttackSpeedMultiplier;
	ATTRIBUTE_ACCESSORS(UOBPlasmaRifleAttributeSet, WeaponAttackSpeedMultiplier);

	// 무기 전용 고유 공격 범위/사거리 배율 (기본 1.0f -> 거대 장판/초광역 기믹용)
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Multiplier")
	FGameplayAttributeData WeaponRangeMultiplier;
	ATTRIBUTE_ACCESSORS(UOBPlasmaRifleAttributeSet, WeaponRangeMultiplier);

	// =====================================================================
	// [고정 값 더하기용 보너스(Additive)]
	// =====================================================================
	// 무기 고유 고정 데미지 추가량 (ex - 증강으로 데미지 +5 고정 증가 시)
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Bonus")
	FGameplayAttributeData BonusDamage;
	ATTRIBUTE_ACCESSORS(UOBPlasmaRifleAttributeSet, BonusDamage);

	// 무기 고유 고정 공격 딜레이 감소량 (초 단위 고정 차감선)
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Bonus")
	FGameplayAttributeData BonusAttackDelay;
	ATTRIBUTE_ACCESSORS(UOBPlasmaRifleAttributeSet, BonusAttackDelay);

	// 무기 고유 고정 공격 범위/사거리 추가량 (cm 단위 고정 확장선)
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Bonus")
	FGameplayAttributeData BonusRange;
	ATTRIBUTE_ACCESSORS(UOBPlasmaRifleAttributeSet, BonusRange);

	// =====================================================================
	// [패시브 계열 공통 속성선]
	// =====================================================================
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Passive")
	FGameplayAttributeData PassiveDamage;
	ATTRIBUTE_ACCESSORS(UOBPlasmaRifleAttributeSet, PassiveDamage);

	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Passive")
	FGameplayAttributeData PassiveAttackDelay;
	ATTRIBUTE_ACCESSORS(UOBPlasmaRifleAttributeSet, PassiveAttackDelay);

	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Passive")
	FGameplayAttributeData PassiveAttackRange;
	ATTRIBUTE_ACCESSORS(UOBPlasmaRifleAttributeSet, PassiveAttackRange);

	// =====================================================================
	// [액티브 계열 공통 속성선]
	// =====================================================================
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Selected")
	FGameplayAttributeData SelectedDamage;
	ATTRIBUTE_ACCESSORS(UOBPlasmaRifleAttributeSet, SelectedDamage);

	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Selected")
	FGameplayAttributeData SelectedAttackDelay;
	ATTRIBUTE_ACCESSORS(UOBPlasmaRifleAttributeSet, SelectedAttackDelay);

	// =====================================================================
	// [공통 유틸리티 속성]
	// =====================================================================
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Common")
	FGameplayAttributeData CommonProjectileRange;
	ATTRIBUTE_ACCESSORS(UOBPlasmaRifleAttributeSet, CommonProjectileRange);

	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Common")
	FGameplayAttributeData CommonAttackSpeed;
	ATTRIBUTE_ACCESSORS(UOBPlasmaRifleAttributeSet, CommonAttackSpeed);
};