#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "OBSpawnManager.generated.h"

USTRUCT(BlueprintType)
struct FRuntimeSpawnRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnData")
	float StartTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnData")
	float EndTime = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnData")
	FGameplayTag EnemyPoolTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnData")
	float SpawnInterval = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnData")
	int32 SpawnAmount = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnData")
	int32 MonsterLevel = 1;

	float LastSpawnTime = -999.0f;
};

UCLASS()
class OUTBREAKBREAKER_API AOBSpawnManager : public AActor
{
	GENERATED_BODY()

public:
	AOBSpawnManager();

	FORCEINLINE void SetSpawnTimelineActive(bool bActive) { bIsTimelineActive = bActive; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void CheckSpawnTimelineLoop();
	void SpawnEnemyWave(const FGameplayTag PoolTag, int32 Amount, int32 InLevel);

	void RotateAndRefreshSpawnLocationCache();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn|Data")
	TObjectPtr<UDataTable> SpawnDataTable;

private:
	float TotalElapsedTime = 0.0f;

	FTimerHandle SpawnTimerHandle;

	UPROPERTY()
	TObjectPtr<AActor> TargetPlayer = nullptr;

	bool bIsTimelineActive = true;

	TArray<FVector> PrecalculatedLocations;

	const int32 MaxLocationCacheSize = 100;

	int32 CurrentRotationIndex = 0;
};
