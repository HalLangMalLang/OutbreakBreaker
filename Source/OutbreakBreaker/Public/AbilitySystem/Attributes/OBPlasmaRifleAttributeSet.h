#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/OBWeaponAttributeSetBase.h"
#include "OBPlasmaRifleAttributeSet.generated.h"

UCLASS()
class OUTBREAKBREAKER_API UOBPlasmaRifleAttributeSet : public UOBWeaponAttributeSetBase
{
	GENERATED_BODY()
	
public:
	// 플라즈마 라이플 전용 기본 데미지
	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Plasma Rifle")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UOBPlasmaRifleAttributeSet, Damage);

	// 데미지 틱 간격
	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Plasma Rifle")
	FGameplayAttributeData TickInterval;
	ATTRIBUTE_ACCESSORS(UOBPlasmaRifleAttributeSet, TickInterval);

	// 대기 상태 : 자동 조준 사거리
	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Plasma Rifle")
	FGameplayAttributeData PassiveRange;
	ATTRIBUTE_ACCESSORS(UOBPlasmaRifleAttributeSet, PassiveRange);

	// 선택 상태 : 관통 레이저가 뚫는 적 수
	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Plasma Rifle")
	FGameplayAttributeData PenetrationCount;
	ATTRIBUTE_ACCESSORS(UOBPlasmaRifleAttributeSet, PenetrationCount);
};
