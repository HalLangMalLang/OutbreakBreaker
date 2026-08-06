#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "OBWeaponAttributeSetBase.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class OUTBREAKBREAKER_API UOBWeaponAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	// 6종 무기 공통 : 기본 데미지 (Rifle의 지속뎀, Hammer의 타격뎀, Shotgun의 펠릿뎀, Drone/Mine/Aura의 기본 데미지 범용 처리)
	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Weapon Base")
	FGameplayAttributeData BaseDamage;
	ATTRIBUTE_ACCESSORS(UOBWeaponAttributeSetBase, BaseDamage);
};
