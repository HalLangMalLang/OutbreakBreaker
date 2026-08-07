#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/OBWeaponAttributeSetBase.h"
#include "OBAuraModuleAttributeSet.generated.h"

UCLASS()
class OUTBREAKBREAKER_API UOBAuraModuleAttributeSet : public UOBWeaponAttributeSetBase
{
	GENERATED_BODY()
	
public:
	// 아우라 전용 틱당 지속 데미지
	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Aura Module", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData TickDamage;
	ATTRIBUTE_ACCESSORS(UOBAuraModuleAttributeSet, TickDamage);

	// 공통 상태 : 적용 반경
	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Aura Module")
	FGameplayAttributeData AuraRadius;
	ATTRIBUTE_ACCESSORS(UOBAuraModuleAttributeSet, AuraRadius);

	// 공통 상태 : 데미지 틱 간격
	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Aura Module")
	FGameplayAttributeData TickInterval;
	ATTRIBUTE_ACCESSORS(UOBAuraModuleAttributeSet, TickInterval);
};
