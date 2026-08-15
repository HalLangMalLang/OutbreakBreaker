#pragma once

#include "CoreMinimal.h"
#include "Character/OBCharacterBase.h"
#include "GameplayTagContainer.h"
#include "Core/PoolableInterface.h"
#include "Interface/OBSpawnableInterface.h"
#include "OBEnemy.generated.h"

class AOBEffectActor;

UCLASS()
class OUTBREAKBREAKER_API AOBEnemy : public AOBCharacterBase, public IPoolableInterface, public IOBSpawnableInterface
{
	GENERATED_BODY()

public:
	AOBEnemy();

	virtual void OnSpawnFromPool() override;
	virtual void OnRecycleToPool() override;

	virtual void InitializeSpawnedObject(int32 InLevel, AActor* InTargetPlayer) override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void InitAbilityActorInfo() override;
	virtual void InitializeAttributeDelegates() override;
	virtual void OnCharacterDeathProcessed(AActor* Destroyer) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|Drop")
	TSubclassOf<AOBEffectActor> XPGemClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> LevelInitEffect;

	UPROPERTY(BlueprintReadWrite, Category = "GAS|EnemyAI")
	TObjectPtr<AActor> TargetPlayer = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "GAS|EnemyAI")
	bool bIsDead = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|EnemyAI")
	float AttackRange = 150.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|EnemyAI")
	FGameplayTag AttackAbilityTag;
};
