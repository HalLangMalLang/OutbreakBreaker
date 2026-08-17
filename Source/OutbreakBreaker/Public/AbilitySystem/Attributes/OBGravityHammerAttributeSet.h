#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/OBWeaponAttributeSetBase.h"
#include "OBGravityHammerAttributeSet.generated.h"

UCLASS()
class OUTBREAKBREAKER_API UOBGravityHammerAttributeSet : public UOBWeaponAttributeSetBase
{
	GENERATED_BODY()

public:
	FORCEINLINE float GetFinalSelectedPullRadius() const { return GetSelectedPullRadius() + GetBonusPullRadius(); }
	FORCEINLINE float GetFinalPassivePullRadius() const { return GetPassivePullRadius() + GetBonusPullRadius(); }

public:
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Passive")
	FGameplayAttributeData PassivePullRadius;
	ATTRIBUTE_ACCESSORS(UOBGravityHammerAttributeSet, PassivePullRadius);

	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Active")
	FGameplayAttributeData SelectedPullRadius;
	ATTRIBUTE_ACCESSORS(UOBGravityHammerAttributeSet, SelectedPullRadius);

	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Common")
	FGameplayAttributeData CommonKnockbackForce;
	ATTRIBUTE_ACCESSORS(UOBGravityHammerAttributeSet, CommonKnockbackForce);

	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Bonus")
	FGameplayAttributeData BonusPullRadius;
	ATTRIBUTE_ACCESSORS(UOBGravityHammerAttributeSet, BonusPullRadius);
};
