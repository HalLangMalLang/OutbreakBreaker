#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/OBWeaponAttributeSetBase.h"
#include "OBDefenseDroneAttributeSet.generated.h"

UCLASS()
class OUTBREAKBREAKER_API UOBDefenseDroneAttributeSet : public UOBWeaponAttributeSetBase
{
	GENERATED_BODY()

public:
	FORCEINLINE float GetFinalPassiveOrbitRadius() const { return GetPassiveOrbitRadius() + GetBonusOrbitRadius(); }

public:
	// 캐릭터 주변 공전 회전 반경
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Passive")
	FGameplayAttributeData PassiveOrbitRadius;
	ATTRIBUTE_ACCESSORS(UOBDefenseDroneAttributeSet, PassiveOrbitRadius);

	// 투사체를 튕겨내거나 소멸시키는 감시 딜레이 주기
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Passive")
	FGameplayAttributeData PassiveDeflectProjectileDelay;
	ATTRIBUTE_ACCESSORS(UOBDefenseDroneAttributeSet, PassiveDeflectProjectileDelay);

	// 근처에 접근한 적을 강제로 넉백 장벽으로 밀쳐내는 딜레이 주기
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Passive")
	FGameplayAttributeData PassiveDeflectEnemyDelay;
	ATTRIBUTE_ACCESSORS(UOBDefenseDroneAttributeSet, PassiveDeflectEnemyDelay);

	// 좀비를 밀쳐낼 때 적용할 물리적 넉백 힘의 밀도
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Passive")
	FGameplayAttributeData PassivePushForce;
	ATTRIBUTE_ACCESSORS(UOBDefenseDroneAttributeSet, PassivePushForce);

	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Bonus")
	FGameplayAttributeData BonusOrbitRadius;
	ATTRIBUTE_ACCESSORS(UOBDefenseDroneAttributeSet, BonusOrbitRadius);
};
