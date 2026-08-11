#include "Player/OBPlayerState.h"
#include "AbilitySystem/Attributes/OBCharacterAttributeSet.h"
#include "AbilitySystem/OBAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/OBPlasmaRifleAttributeSet.h"
#include "AbilitySystem/Attributes/OBGravityHammerAttributeSet.h"
#include "AbilitySystem/Attributes/OBSpreadShotgunAttributeSet.h"
#include "AbilitySystem/Attributes/OBDefenseDroneAttributeSet.h"
#include "AbilitySystem/Attributes/OBMagnetMineAttributeSet.h"
#include "AbilitySystem/Attributes/OBAuraModuleAttributeSet.h"

AOBPlayerState::AOBPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UOBAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	AbilitySystemComponent->SetIsReplicated(false);
	CharacterAttributeSet = CreateDefaultSubobject<UOBCharacterAttributeSet>(TEXT("CharacterAttributeSet"));

	// 주무기 3종 세트 생성
	PlasmaRifleAttributes = CreateDefaultSubobject<UOBPlasmaRifleAttributeSet>(TEXT("PlasmaRifleAttributes"));
	GravityHammerAttributes = CreateDefaultSubobject<UOBGravityHammerAttributeSet>(TEXT("GravityHammerAttributes"));
	SpreadShotgunAttributes = CreateDefaultSubobject<UOBSpreadShotgunAttributeSet>(TEXT("SpreadShotgunAttributes"));

	// 보조무기 3종 세트 생성
	DefenseDroneAttributes = CreateDefaultSubobject<UOBDefenseDroneAttributeSet>(TEXT("DefenseDroneAttributes"));
	MagnetMineAttributes = CreateDefaultSubobject<UOBMagnetMineAttributeSet>(TEXT("MagnetMineAttributes"));
	AuraModuleAttributes = CreateDefaultSubobject<UOBAuraModuleAttributeSet>(TEXT("AuraModuleAttributes"));
}

UAbilitySystemComponent* AOBPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
