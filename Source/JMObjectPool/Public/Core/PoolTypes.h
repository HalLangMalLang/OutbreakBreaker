// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PoolTypes.generated.h"

class UPooledObject;

USTRUCT(BlueprintType)
struct FPooledObjectData
{
	GENERATED_BODY()

	FPooledObjectData() {}

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ActorTemplate = nullptr;

	UPROPERTY(EditAnywhere)
	int32 PoolSize = 1;

	UPROPERTY(EditAnywhere)
	bool bCanGrow = false;
};

USTRUCT(BlueprintType)
struct FSingleObjectPool
{
	GENERATED_BODY()

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	TArray<TObjectPtr<UPooledObject>> PooledObjects;
};