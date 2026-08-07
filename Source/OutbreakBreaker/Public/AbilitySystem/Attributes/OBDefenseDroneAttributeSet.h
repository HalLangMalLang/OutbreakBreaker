#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/OBWeaponAttributeSetBase.h"
#include "OBDefenseDroneAttributeSet.generated.h"

UCLASS()
class OUTBREAKBREAKER_API UOBDefenseDroneAttributeSet : public UOBWeaponAttributeSetBase
{
	GENERATED_BODY()
	
public:
	// 공통 상태 : 접근 적 밀쳐내는 힘
	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Defense Drone")
	FGameplayAttributeData PushForce;
	ATTRIBUTE_ACCESSORS(UOBDefenseDroneAttributeSet, PushForce);

	// 공통 상태 : 캐릭터 주변 회전 반경
	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Defense Drone")
	FGameplayAttributeData OrbitRadius;
	ATTRIBUTE_ACCESSORS(UOBDefenseDroneAttributeSet, OrbitRadius);

	// 공통 상태 : 투사체 방어 재사용 대기시간
	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Defense Drone")
	FGameplayAttributeData DeflectCooldown;
	ATTRIBUTE_ACCESSORS(UOBDefenseDroneAttributeSet, DeflectCooldown);
};
