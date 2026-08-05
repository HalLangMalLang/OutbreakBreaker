// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PooledObject.generated.h"

class UObjectPoolSubsystem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class JMOBJECTPOOL_API UPooledObject : public UActorComponent
{
	GENERATED_BODY()

public:
	void Init(UObjectPoolSubsystem* Owner);

	UFUNCTION(BlueprintCallable)
	void RecycleSelf();

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	bool bIsPoolActive;

private:
	UPROPERTY()
	TObjectPtr<UObjectPoolSubsystem> ObjectPool;

	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

};