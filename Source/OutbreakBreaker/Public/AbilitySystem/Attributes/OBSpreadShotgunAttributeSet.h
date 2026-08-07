#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/OBWeaponAttributeSetBase.h"
#include "OBSpreadShotgunAttributeSet.generated.h"

UCLASS()
class OUTBREAKBREAKER_API UOBSpreadShotgunAttributeSet : public UOBWeaponAttributeSetBase
{
	GENERATED_BODY()
	
public:
	// 스프레드 샷건 전용 산탄 한 알당 데미지
	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Spread Shotgun", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData DamagePerPellet;
	ATTRIBUTE_ACCESSORS(UOBSpreadShotgunAttributeSet, DamagePerPellet);

	// 공통 상태 : 발사되는 산탄 수
	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Spread Shotgun")
	FGameplayAttributeData PelletCount;
	ATTRIBUTE_ACCESSORS(UOBSpreadShotgunAttributeSet, PelletCount);

	// 선택 상태 : 부채꼴 발사 각도
	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Spread Shotgun")
	FGameplayAttributeData SpreadAngle;
	ATTRIBUTE_ACCESSORS(UOBSpreadShotgunAttributeSet, SpreadAngle);

	// 공통 상태 : 재발사 대기시간/쿨타임
	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Spread Shotgun")
	FGameplayAttributeData FireCooldown;
	ATTRIBUTE_ACCESSORS(UOBSpreadShotgunAttributeSet, FireCooldown);
};
