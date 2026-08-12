#pragma once

#include "CoreMinimal.h"
#include "Character/OBCharacterBase.h"
#include "OBEnemy.generated.h"

UCLASS()
class OUTBREAKBREAKER_API AOBEnemy : public AOBCharacterBase
{
	GENERATED_BODY()

public:
	AOBEnemy();

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeAttributeDelegates() override;

	virtual void OnCharacterDeathProcessed(AActor* Destroyer) override;
};
