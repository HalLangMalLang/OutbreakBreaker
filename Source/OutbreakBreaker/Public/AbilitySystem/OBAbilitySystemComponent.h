#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "OBAbilitySystemComponent.generated.h"


UCLASS()
class OUTBREAKBREAKER_API UOBAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);

	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
};
