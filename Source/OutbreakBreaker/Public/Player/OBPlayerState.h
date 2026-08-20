#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "OBPlayerState.generated.h"

class UAttributeSet;
class UOBWeaponAttributeSetBase;

UCLASS()
class OUTBREAKBREAKER_API AOBPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AOBPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	template<typename T>
	T* GetAttributeSetOfClass() const;

public:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> CharacterAttributeSet;

	/** 주무기 3종 고유 속성 세트 */
	UPROPERTY(VisibleAnywhere, Category = "GAS | Attributes | Primary")
	TObjectPtr<UOBWeaponAttributeSetBase> PlasmaRifleAttributes;

	UPROPERTY(VisibleAnywhere, Category = "GAS | Attributes | Primary")
	TObjectPtr<UOBWeaponAttributeSetBase> GravityHammerAttributes;

	UPROPERTY(VisibleAnywhere, Category = "GAS | Attributes | Primary")
	TObjectPtr<UOBWeaponAttributeSetBase> SpreadShotgunAttributes;

	/** 보조무기 3종 고유 속성 세트 */
	UPROPERTY(VisibleAnywhere, Category = "GAS | Attributes | Secondary")
	TObjectPtr<UOBWeaponAttributeSetBase> DefenseDroneAttributes;

	UPROPERTY(VisibleAnywhere, Category = "GAS | Attributes | Secondary")
	TObjectPtr<UOBWeaponAttributeSetBase> MagnetMineAttributes;

	UPROPERTY(VisibleAnywhere, Category = "GAS | Attributes | Secondary")
	TObjectPtr<UOBWeaponAttributeSetBase> AuraModuleAttributes;
};

template<typename T>
inline T* AOBPlayerState::GetAttributeSetOfClass() const
{
	if (!AbilitySystemComponent)
	{
		return nullptr;
	}

	const UAttributeSet* AS = AbilitySystemComponent->GetAttributeSet(T::StaticClass());

	return const_cast<T*>(Cast<T>(AS));
}
