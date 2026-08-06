#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/OBWeaponAttributeSetBase.h"
#include "OBSpreadShotgunAttributeSet.generated.h"

UCLASS()
class OUTBREAKBREAKER_API UOBSpreadShotgunAttributeSet : public UOBWeaponAttributeSetBase
{
	GENERATED_BODY()
	
public:
	// 공통 상태 : 발사되는 산탄 수 (산탄 수 증가 강화)
	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Spread Shotgun")
	FGameplayAttributeData PelletCount;
	ATTRIBUTE_ACCESSORS(UOBSpreadShotgunAttributeSet, PelletCount);

	// 선택 상태 : 부채꼴 발사 각도 (각도 조정 강화)
	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Spread Shotgun")
	FGameplayAttributeData SpreadAngle;
	ATTRIBUTE_ACCESSORS(UOBSpreadShotgunAttributeSet, SpreadAngle);

	// 공통 상태 : 재발사 대기시간/쿨타임 (쿨타임 감소 강화)
	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Spread Shotgun")
	FGameplayAttributeData FireCooldown;
	ATTRIBUTE_ACCESSORS(UOBSpreadShotgunAttributeSet, FireCooldown);
};
