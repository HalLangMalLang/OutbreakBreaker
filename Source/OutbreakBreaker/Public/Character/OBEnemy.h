#pragma once

#include "CoreMinimal.h"
#include "Character/OBCharacterBase.h"
#include "GameplayTagContainer.h"
#include "OBEnemy.generated.h"

class AOBEffectActor;

UCLASS()
class OUTBREAKBREAKER_API AOBEnemy : public AOBCharacterBase
{
	GENERATED_BODY()

public:
	AOBEnemy();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void InitAbilityActorInfo() override;
	virtual void InitializeAttributeDelegates() override;
	virtual void OnCharacterDeathProcessed(AActor* Destroyer) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|Drop")
	TSubclassOf<AOBEffectActor> XPGemClass;

	UPROPERTY(BlueprintReadWrite, Category = "GAS|EnemyAI")
	TObjectPtr<AActor> TargetPlayer = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "GAS|EnemyAI")
	bool bIsDead = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|EnemyAI")
	float AttackRange = 150.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|EnemyAI")
	FGameplayTag AttackAbilityTag;
};
