#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/OBWeaponAttributeSetBase.h"
#include "OBSpreadShotgunAttributeSet.generated.h"

UCLASS()
class OUTBREAKBREAKER_API UOBSpreadShotgunAttributeSet : public UOBWeaponAttributeSetBase
{
	GENERATED_BODY()

public:
	FORCEINLINE float GetFinalSelectedPelletCount() const { return GetSelectedPelletCount() + GetBonusPelletCount(); }
	FORCEINLINE float GetFinalPassivePelletCount() const { return GetPassivePelletCount() + GetBonusPelletCount(); }

public:
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Passive")
	FGameplayAttributeData PassivePelletCount;
	ATTRIBUTE_ACCESSORS(UOBSpreadShotgunAttributeSet, PassivePelletCount);

	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Active")
	FGameplayAttributeData SelectedPelletCount;
	ATTRIBUTE_ACCESSORS(UOBSpreadShotgunAttributeSet, SelectedPelletCount);

	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Common")
	FGameplayAttributeData CommonSpreadAngle;
	ATTRIBUTE_ACCESSORS(UOBSpreadShotgunAttributeSet, CommonSpreadAngle);

	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Common")
	FGameplayAttributeData CommonProjectileRange;
	ATTRIBUTE_ACCESSORS(UOBSpreadShotgunAttributeSet, CommonProjectileRange);

	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Bonus")
	FGameplayAttributeData BonusPelletCount;
	ATTRIBUTE_ACCESSORS(UOBSpreadShotgunAttributeSet, BonusPelletCount);
};
