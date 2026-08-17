#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/OBWeaponAttributeSetBase.h"
#include "OBAuraModuleAttributeSet.generated.h"

UCLASS()
class OUTBREAKBREAKER_API UOBAuraModuleAttributeSet : public UOBWeaponAttributeSetBase
{
	GENERATED_BODY()

public:
	FORCEINLINE float GetFinalPassiveSlowAreaRadius() const { return GetPassiveSlowAreaRadius() + GetBonusSlowAreaRadius(); }

public:
	// 오라 장판 적을 둔화시키는 슬로우 영역의 총 반경 크기
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Passive")
	FGameplayAttributeData PassiveSlowAreaRadius;
	ATTRIBUTE_ACCESSORS(UOBAuraModuleAttributeSet, PassiveSlowAreaRadius);

	// 오라 장판 딜레이 
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Passive")
	FGameplayAttributeData PassiveSlowAreaDelay;
	ATTRIBUTE_ACCESSORS(UOBAuraModuleAttributeSet, PassiveSlowAreaDelay);

	// 오라 장판의 필드 지속 시간
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Passive")
	FGameplayAttributeData PassiveSlowAreaTimer;
	ATTRIBUTE_ACCESSORS(UOBAuraModuleAttributeSet, PassiveSlowAreaTimer);

	// 오라 장판 영역 밟은 적의 슬로우 감속률
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Passive")
	FGameplayAttributeData PassiveSlowAreaAmount;
	ATTRIBUTE_ACCESSORS(UOBAuraModuleAttributeSet, PassiveSlowAreaAmount);

	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Bonus")
	FGameplayAttributeData BonusSlowAreaRadius;
	ATTRIBUTE_ACCESSORS(UOBAuraModuleAttributeSet, BonusSlowAreaRadius);
};
