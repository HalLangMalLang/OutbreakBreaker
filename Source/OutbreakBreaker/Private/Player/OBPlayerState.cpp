#include "Player/OBPlayerState.h"
#include "AbilitySystem/Attributes/OBCharacterAttributeSet.h"
#include "AbilitySystem/OBAbilitySystemComponent.h"

AOBPlayerState::AOBPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UOBAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(false);
	AttributeSet = CreateDefaultSubobject<UOBCharacterAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* AOBPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
