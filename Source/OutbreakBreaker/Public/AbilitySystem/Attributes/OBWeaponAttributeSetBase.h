#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "OBWeaponAttributeSetBase.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class OUTBREAKBREAKER_API UOBWeaponAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()

public:
	// 현재 무기의 강화 레벨
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Control")
	FGameplayAttributeData WeaponLevel;
	ATTRIBUTE_ACCESSORS(UOBWeaponAttributeSetBase, WeaponLevel);

	// =====================================================================
	// [곱연산 배율 스케일링(Multiplier) 주머니 세트]
	// =====================================================================
	// 무기 전용 고유 공격력 배율 (기본 1.0f -> 증강/시너지 시 곱연산 스케일링)
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Multiplier")
	FGameplayAttributeData WeaponDamageMultiplier;
	ATTRIBUTE_ACCESSORS(UOBWeaponAttributeSetBase, WeaponDamageMultiplier);

	// 무기 전용 고유 공격속도 배율 (기본 1.0f -> 개별 무기 속사포 튜닝선)
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Multiplier")
	FGameplayAttributeData WeaponAttackSpeedMultiplier;
	ATTRIBUTE_ACCESSORS(UOBWeaponAttributeSetBase, WeaponAttackSpeedMultiplier);

	// 무기 전용 고유 공격 범위/사거리 배율 (기본 1.0f -> 거대 장판/초광역 기믹용)
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Multiplier")
	FGameplayAttributeData WeaponRangeMultiplier;
	ATTRIBUTE_ACCESSORS(UOBWeaponAttributeSetBase, WeaponRangeMultiplier);

	// =====================================================================
	// 고정 값 더하기용 보너스(Additive)
	// =====================================================================
	// 무기 고유 고정 데미지 추가량 (ex - 증강으로 데미지 +5 고정 증가 시)
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Bonus")
	FGameplayAttributeData BonusDamage;
	ATTRIBUTE_ACCESSORS(UOBWeaponAttributeSetBase, BonusDamage);

	// 무기 고유 고정 공격 딜레이 감소량 (초 단위 고정 차감선)
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Bonus")
	FGameplayAttributeData BonusAttackDelay;
	ATTRIBUTE_ACCESSORS(UOBWeaponAttributeSetBase, BonusAttackDelay);

	// 무기 고유 고정 공격 범위/사거리 추가량 (cm 단위 고정 확장선)
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Bonus")
	FGameplayAttributeData BonusRange;
	ATTRIBUTE_ACCESSORS(UOBWeaponAttributeSetBase, BonusRange);

	// =====================================================================
	// [패시브 계열 공통 속성선]
	// =====================================================================
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Passive")
	FGameplayAttributeData PassiveDamage;
	ATTRIBUTE_ACCESSORS(UOBWeaponAttributeSetBase, PassiveDamage);

	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Passive")
	FGameplayAttributeData PassiveAttackDelay;
	ATTRIBUTE_ACCESSORS(UOBWeaponAttributeSetBase, PassiveAttackDelay);

	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Passive")
	FGameplayAttributeData PassiveAttackRange;
	ATTRIBUTE_ACCESSORS(UOBWeaponAttributeSetBase, PassiveAttackRange);

	// =====================================================================
	// [액티브 계열 공통 속성선]
	// =====================================================================
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Selected")
	FGameplayAttributeData SelectedDamage;
	ATTRIBUTE_ACCESSORS(UOBWeaponAttributeSetBase, SelectedDamage);

	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Selected")
	FGameplayAttributeData SelectedAttackDelay;
	ATTRIBUTE_ACCESSORS(UOBWeaponAttributeSetBase, SelectedAttackDelay);

	// =====================================================================
	// [공통 유틸리티 속성]
	// =====================================================================
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Common")
	FGameplayAttributeData CommonAttackSpeed;
	ATTRIBUTE_ACCESSORS(UOBWeaponAttributeSetBase, CommonAttackSpeed);
};
