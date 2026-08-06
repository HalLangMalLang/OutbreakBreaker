#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/OBWeaponAttributeSetBase.h"
#include "OBAuraModuleAttributeSet.generated.h"

UCLASS()
class OUTBREAKBREAKER_API UOBAuraModuleAttributeSet : public UOBWeaponAttributeSetBase
{
	GENERATED_BODY()
	
public:
	// 공통 상태 : 적용 반경 (반경 증가 강화)
	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Aura Module")
	FGameplayAttributeData AuraRadius;
	ATTRIBUTE_ACCESSORS(UOBAuraModuleAttributeSet, AuraRadius);

	// 공통 상태 : 데미지 틱 간격 (간격 감소 강화)
	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Aura Module")
	FGameplayAttributeData TickInterval;
	ATTRIBUTE_ACCESSORS(UOBAuraModuleAttributeSet, TickInterval);
};
