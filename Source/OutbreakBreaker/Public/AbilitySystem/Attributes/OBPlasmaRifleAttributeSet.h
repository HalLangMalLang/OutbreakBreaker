#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/OBWeaponAttributeSetBase.h"
#include "OBPlasmaRifleAttributeSet.generated.h"

UCLASS()
class OUTBREAKBREAKER_API UOBPlasmaRifleAttributeSet : public UOBWeaponAttributeSetBase
{
	GENERATED_BODY()
	
public:
	// 데미지 틱 간격 (감소 강화)
	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Plasma Rifle")
	FGameplayAttributeData TickInterval;
	ATTRIBUTE_ACCESSORS(UOBPlasmaRifleAttributeSet, TickInterval);

	// 대기 상태 : 자동 조준 사거리 (증가 강화)
	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Plasma Rifle")
	FGameplayAttributeData PassiveRange;
	ATTRIBUTE_ACCESSORS(UOBPlasmaRifleAttributeSet, PassiveRange);

	// 선택 상태 : 관통 레이저가 뚫는 적 수 (증가 강화)
	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Plasma Rifle")
	FGameplayAttributeData PenetrationCount;
	ATTRIBUTE_ACCESSORS(UOBPlasmaRifleAttributeSet, PenetrationCount);
};
