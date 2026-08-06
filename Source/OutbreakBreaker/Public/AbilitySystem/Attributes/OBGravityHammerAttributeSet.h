#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/OBWeaponAttributeSetBase.h"
#include "OBGravityHammerAttributeSet.generated.h"

UCLASS()
class OUTBREAKBREAKER_API UOBGravityHammerAttributeSet : public UOBWeaponAttributeSetBase
{
	GENERATED_BODY()
	
public:
	// 대기 상태 : 바닥 찍기 주기/쿨타임 (감소 강화)
	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Gravity Hammer")
	FGameplayAttributeData SlamCooldown;
	ATTRIBUTE_ACCESSORS(UOBGravityHammerAttributeSet, SlamCooldown);

	// 공통 상태 : 넉백 힘 (넉백 강화)
	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Gravity Hammer")
	FGameplayAttributeData KnockbackForce;
	ATTRIBUTE_ACCESSORS(UOBGravityHammerAttributeSet, KnockbackForce);

	// 선택 상태 : 적 끌어들이기 반경 (반경 증가 강화)
	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Gravity Hammer")
	FGameplayAttributeData PullRadius;
	ATTRIBUTE_ACCESSORS(UOBGravityHammerAttributeSet, PullRadius);
};
