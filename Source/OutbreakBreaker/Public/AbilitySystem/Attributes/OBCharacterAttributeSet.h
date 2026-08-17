#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/OBAttributeSetBase.h"
#include "OBCharacterAttributeSet.generated.h"

UCLASS()
class OUTBREAKBREAKER_API UOBCharacterAttributeSet : public UOBAttributeSetBase
{
	GENERATED_BODY()

protected:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void HandleDeathNotification(AActor* Destroyer);
	virtual void HandleLevelUpNotification(float NewLevle);
	virtual void HandleSpeedChangedNotification(float NewSpeed);

public:
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData XP;
	ATTRIBUTE_ACCESSORS(UOBCharacterAttributeSet, XP);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxXP;
	ATTRIBUTE_ACCESSORS(UOBCharacterAttributeSet, MaxXP);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData AttackPowerMultiplier;
	ATTRIBUTE_ACCESSORS(UOBCharacterAttributeSet, AttackPowerMultiplier);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData AttackSpeedMultiplier;
	ATTRIBUTE_ACCESSORS(UOBCharacterAttributeSet, AttackSpeedMultiplier);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData HPMultiplier;
	ATTRIBUTE_ACCESSORS(UOBCharacterAttributeSet, HPMultiplier);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData XPMultiplier;
	ATTRIBUTE_ACCESSORS(UOBCharacterAttributeSet, XPMultiplier);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData BonusHP;
	ATTRIBUTE_ACCESSORS(UOBCharacterAttributeSet, BonusHP);


	FOBOnLevelUpSignature OnLevelUpDelegate;
	FOBOnCharacterDeadSignature OnCharacterDeadDelegate;
	FOBOnSpeedChangedSignature OnSpeedChangedDelegate;
};

