#pragma once

#include "CoreMinimal.h"
#include "PoolTypes.generated.h"

class UPooledObject;

USTRUCT(BlueprintType)
struct FPooledObjectData
{
	GENERATED_BODY()

	FPooledObjectData() {}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pool", meta = (MustImplement = "/Script/JMObjectPool.PoolableInterface"))
	TSubclassOf<AActor> ActorTemplate = nullptr;

	UPROPERTY(EditAnywhere, Category = "Pool")
	int32 PoolSize = 1;

	UPROPERTY(EditAnywhere, Category = "Pool")
	bool bCanGrow = false;
};

USTRUCT(BlueprintType)
struct FSingleObjectPool
{
	GENERATED_BODY()

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	TArray<TObjectPtr<UPooledObject>> PooledObjects;
};