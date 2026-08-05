#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "OBGameplayAbility.generated.h"

UCLASS()
class OUTBREAKBREAKER_API UOBGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag;
};
