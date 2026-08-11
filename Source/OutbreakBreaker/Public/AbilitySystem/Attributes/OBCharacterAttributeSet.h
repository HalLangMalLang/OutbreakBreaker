#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "OBCharacterAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties() {}

	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;
	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;
	UPROPERTY()
	AController* SourceController = nullptr;
	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;
	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;
	UPROPERTY()
	AController* TargetController = nullptr;
	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOBOnLevelUpSignature, float);

DECLARE_MULTICAST_DELEGATE_OneParam(FOBOnCharacterDeadSignature, AActor*);

DECLARE_MULTICAST_DELEGATE_OneParam(FOBOnSpeedChangedSignature, float);

UCLASS()
class OUTBREAKBREAKER_API UOBCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const;

public:
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData HP;
	ATTRIBUTE_ACCESSORS(UOBCharacterAttributeSet, HP);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxHP;
	ATTRIBUTE_ACCESSORS(UOBCharacterAttributeSet, MaxHP);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData XP;
	ATTRIBUTE_ACCESSORS(UOBCharacterAttributeSet, XP);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxXP;
	ATTRIBUTE_ACCESSORS(UOBCharacterAttributeSet, MaxXP);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Level;
	ATTRIBUTE_ACCESSORS(UOBCharacterAttributeSet, Level);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(UOBCharacterAttributeSet, MoveSpeed);

	FOBOnLevelUpSignature OnLevelUpDelegate;
	FOBOnCharacterDeadSignature OnCharacterDeadDelegate;
	FOBOnSpeedChangedSignature OnSpeedChangedDelegate;
};
