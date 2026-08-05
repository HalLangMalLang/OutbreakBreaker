#include "Player/OBPlayerState.h"
#include "AbilitySystem/OBAttributeSet.h"
#include "AbilitySystem/OBAbilitySystemComponent.h"

AOBPlayerState::AOBPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UOBAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(false);
	AttributeSet = CreateDefaultSubobject<UOBAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* AOBPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
