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

	FOBOnCharacterDeadSignature OnEnemyDeadDelegate;
	FOBOnSpeedChangedSignature OnSpeedEnemyDelegate;
};
