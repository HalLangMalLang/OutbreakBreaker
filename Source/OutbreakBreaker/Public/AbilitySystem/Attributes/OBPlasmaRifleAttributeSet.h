#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/OBWeaponAttributeSetBase.h"
#include "OBPlasmaRifleAttributeSet.generated.h"

UCLASS()
class OUTBREAKBREAKER_API UOBPlasmaRifleAttributeSet : public UOBWeaponAttributeSetBase
{
	GENERATED_BODY()

public:
	FORCEINLINE float GetFinalSelectedPenetrationCount() const { return GetSelectedPenetrationCount() + GetBonusPenetrationCount(); }

public:
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Active")
	FGameplayAttributeData SelectedPenetrationCount;
	ATTRIBUTE_ACCESSORS(UOBPlasmaRifleAttributeSet, SelectedPenetrationCount);

	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Common")
	FGameplayAttributeData CommonProjectileRange;
	ATTRIBUTE_ACCESSORS(UOBPlasmaRifleAttributeSet, CommonProjectileRange);

	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Bonus")
	FGameplayAttributeData BonusPenetrationCount;
	ATTRIBUTE_ACCESSORS(UOBPlasmaRifleAttributeSet, BonusPenetrationCount);
};
