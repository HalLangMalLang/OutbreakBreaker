#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "OBPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UAbilitySystemComponent;
class UAttributeSet;
class UOBCharacterAttributeSet;
class UOBPlasmaRifleAttributeSet;
class UOBGravityHammerAttributeSet;
class UOBSpreadShotgunAttributeSet;
class UOBDefenseDroneAttributeSet;
class UOBMagnetMineAttributeSet;
class UOBAuraModuleAttributeSet;

UCLASS()
class OUTBREAKBREAKER_API AOBPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AOBPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	UOBPlasmaRifleAttributeSet* GetPlasmaRifleAttributeSet() const { return PlasmaRifleAttributes; }
	UOBGravityHammerAttributeSet* GetGravityHammerAttributeSet() const { return GravityHammerAttributes; }
	UOBSpreadShotgunAttributeSet* GetSpreadShotgunAttributeSet() const { return SpreadShotgunAttributes; }
	UOBDefenseDroneAttributeSet* GetDefenseDroneAttributeSet() const { return DefenseDroneAttributes; }
	UOBMagnetMineAttributeSet* GetMagnetMineAttributeSet() const { return MagnetMineAttributes; }
	UOBAuraModuleAttributeSet* GetAuraModuleAttributeSet() const { return AuraModuleAttributes; }

	FORCEINLINE int32 GetPlayerLevel() const { return Level; }

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	/** 주무기 3종 고유 속성 세트 */
	UPROPERTY(VisibleAnywhere, Category = "GAS | Attributes | Primary")
	TObjectPtr<UOBPlasmaRifleAttributeSet> PlasmaRifleAttributes;

	UPROPERTY(VisibleAnywhere, Category = "GAS | Attributes | Primary")
	TObjectPtr<UOBGravityHammerAttributeSet> GravityHammerAttributes;

	UPROPERTY(VisibleAnywhere, Category = "GAS | Attributes | Primary")
	TObjectPtr<UOBSpreadShotgunAttributeSet> SpreadShotgunAttributes;

	/** 보조무기 3종 고유 속성 세트 */
	UPROPERTY(VisibleAnywhere, Category = "GAS | Attributes | Secondary")
	TObjectPtr<UOBDefenseDroneAttributeSet> DefenseDroneAttributes;

	UPROPERTY(VisibleAnywhere, Category = "GAS | Attributes | Secondary")
	TObjectPtr<UOBMagnetMineAttributeSet> MagnetMineAttributes;

	UPROPERTY(VisibleAnywhere, Category = "GAS | Attributes | Secondary")
	TObjectPtr<UOBAuraModuleAttributeSet> AuraModuleAttributes;

private:
	UPROPERTY(VisibleAnywhere, Category = "Player Stats")
	int32 Level = 1;
};
