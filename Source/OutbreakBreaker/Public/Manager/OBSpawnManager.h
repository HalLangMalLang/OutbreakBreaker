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

	float LastSpawnTime = -1.f;
};

UCLASS()
class OUTBREAKBREAKER_API AOBSpawnManager : public AActor
{
	GENERATED_BODY()

public:
	AOBSpawnManager();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void InitializeSpawnManager();
	void CheckSpawnTimelineLoop();
	void SpawnEnemyWave(const FGameplayTag PoolTag, int32 Amount, int32 InLevel);
	void RotateAndRefreshSpawnLocationCache();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn|Data")
	TObjectPtr<UDataTable> SpawnDataTable;

	// 플레이어 기준 몬스터가 소환될 수 있는 최소 반경
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OB|SpawnSettings", meta = (ClampMin = "0.0"))
	float MinSpawnRadius = 1300.0f;

	// 플레이어 기준 몬스터가 소환될 수 있는 최대 반경
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OB|SpawnSettings", meta = (ClampMin = "0.0"))
	float MaxSpawnRadius = 1600.0f;

	// 지형(경사로, 계단 등)에서 내비게이션 바닥 판정할 범위
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OB|SpawnSettings")
	FVector NavProjectionExtent = FVector(500.0f, 500.0f, 500.0f);

private:
	float TotalElapsedTime = 0.0f;

	UPROPERTY()
	TObjectPtr<AActor> TargetPlayer = nullptr;

	TArray<FVector> PrecalculatedLocations;

	const int32 MaxLocationCacheSize = 100;

	int32 CurrentRotationIndex = 0;
};
