#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/OBAttributeSetBase.h"
#include "OBEnemyAttributeSet.generated.h"

UCLASS()
class OUTBREAKBREAKER_API UOBEnemyAttributeSet : public UOBAttributeSetBase
{
	GENERATED_BODY()

protected:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void HandleDeathNotification(AActor* Destroyer);
	virtual void HandleSpeedChangedNotification(float NewSpeed);

public:
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UOBEnemyAttributeSet, Damage);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData BountyXP;
	ATTRIBUTE_ACCESSORS(UOBEnemyAttributeSet, BountyXP);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData AttackRange;
	ATTRIBUTE_ACCESSORS(UOBEnemyAttributeSet, AttackRange);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData AttackDelay;
	ATTRIBUTE_ACCESSORS(UOBEnemyAttributeSet, AttackDelay);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData AttackSpeed;
	ATTRIBUTE_ACCESSORS(UOBEnemyAttributeSet, AttackSpeed);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData ProjectileSpeed;
	ATTRIBUTE_ACCESSORS(UOBEnemyAttributeSet, ProjectileSpeed);

	FOBOnCharacterDeadSignature OnEnemyDeadDelegate;
	FOBOnSpeedChangedSignature OnSpeedEnemyDelegate;
};
