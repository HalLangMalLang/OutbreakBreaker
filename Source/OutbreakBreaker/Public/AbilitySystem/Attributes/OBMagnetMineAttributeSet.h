#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/OBWeaponAttributeSetBase.h"
#include "OBMagnetMineAttributeSet.generated.h"

UCLASS()
class OUTBREAKBREAKER_API UOBMagnetMineAttributeSet : public UOBWeaponAttributeSetBase
{
	GENERATED_BODY()

public:
	FORCEINLINE float GetFinalPassiveExplosionRadius() const { return GetPassiveExplosionRadius() + GetBonusExplosionRadius(); }

public:
	// 폭발까지 걸리는 지연 시간
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Passive")
	FGameplayAttributeData PassiveExplosionDelay;
	ATTRIBUTE_ACCESSORS(UOBMagnetMineAttributeSet, PassiveExplosionDelay);

	// 폭발 반경 범위
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Passive")
	FGameplayAttributeData PassiveExplosionRadius;
	ATTRIBUTE_ACCESSORS(UOBMagnetMineAttributeSet, PassiveExplosionRadius);

	// 공격에 맞은 적들의 이동속도를 깎아내릴 감속 배율 (예: 0.3f = 30% 감속)
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Passive")
	FGameplayAttributeData PassiveSlowAmount;
	ATTRIBUTE_ACCESSORS(UOBMagnetMineAttributeSet, PassiveSlowAmount);

	// 슬로우 감속 디버프가 적에 유지될 지속 타이머 시간 (초)
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Passive")
	FGameplayAttributeData PassiveSlowTimer;
	ATTRIBUTE_ACCESSORS(UOBMagnetMineAttributeSet, PassiveSlowTimer);

	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Bonus")
	FGameplayAttributeData BonusExplosionRadius;
	ATTRIBUTE_ACCESSORS(UOBMagnetMineAttributeSet, BonusExplosionRadius);
};
