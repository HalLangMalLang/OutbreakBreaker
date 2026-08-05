// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Core/PoolTypes.h" 
#include "ObjectPoolSubsystem.generated.h"

struct FPooledObjectData;
struct FSingleObjectPool;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPoolerCleanupSignature);

UCLASS()
class JMOBJECTPOOL_API UObjectPoolSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FPoolerCleanupSignature OnPoolerCleanup;

	// [등록] - 풀들을 새로 세팅
	UFUNCTION(BlueprintCallable)
	void SetupPools(const TMap<FName, FPooledObjectData>& InPooledObjectData);

	// [정리] - 기존 풀을 완전 정리
	UFUNCTION(BlueprintCallable)
	void ClearPools();

	UFUNCTION(BlueprintCallable)
	AActor* GetPooledActor(const FName& PoolName);


	UFUNCTION(BlueprintCallable)
	void RecycleActor(AActor* PooledActor);


	UFUNCTION(BlueprintCallable)
	void RecyclePooledObject(UPooledObject* PoolCompRef);


	UFUNCTION(BlueprintCallable)
	void BroadcastPoolerCleanup();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<FName, FPooledObjectData> PooledObjectData;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<FName, FSingleObjectPool> Pools;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

private:
	void RegenItem(const FName& PoolName, int32 PositionIndex);

	// [생성] - 새 월드가 로드 완료되었을 때 호출될 함수
	void OnWorldInitialized(UWorld* World, const UWorld::InitializationValues IValues);

	// [삭제] - 기존 월드가 닫히고 정리될 때 호출될 함수
	void OnWorldCleanup(UWorld* World, bool bSessionEnded, bool bCleanupResources);

	UPROPERTY()
	TObjectPtr<AActor> PoolAnchor;
};
