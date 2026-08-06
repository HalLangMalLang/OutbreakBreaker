#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/OBWeaponAttributeSetBase.h"
#include "OBMagnetMineAttributeSet.generated.h"

UCLASS()
class OUTBREAKBREAKER_API UOBMagnetMineAttributeSet : public UOBWeaponAttributeSetBase
{
	GENERATED_BODY()
	
public:
	// 공통 상태 : 폭발 범위/반경 (범위 증가 강화)
	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Magnet Mine")
	FGameplayAttributeData ExplosionRadius;
	ATTRIBUTE_ACCESSORS(UOBMagnetMineAttributeSet, ExplosionRadius);

	// 공통 상태 : 슬로우 감속률 (감속 강화)
	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Magnet Mine")
	FGameplayAttributeData SlowAmount;
	ATTRIBUTE_ACCESSORS(UOBMagnetMineAttributeSet, SlowAmount);

	// 공통 상태 : 지뢰 투하 주기 (간격 감소 강화)
	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Magnet Mine")
	FGameplayAttributeData DropInterval;
	ATTRIBUTE_ACCESSORS(UOBMagnetMineAttributeSet, DropInterval);
};
