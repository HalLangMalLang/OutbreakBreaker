#pragma once

#include "CoreMinimal.h"
#include "Actor/OBEffectActor.h"
#include "Core/PoolableInterface.h"
#include "Interface/OBSpawnableInterface.h"
#include "OBPooledEffectActor.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS(Abstract, Blueprintable)
class OUTBREAKBREAKER_API AOBPooledEffectActor : public AOBEffectActor, public IPoolableInterface, public IOBSpawnableInterface
{
	GENERATED_BODY()

public:
	AOBPooledEffectActor();

	virtual void OnSpawnFromPool() override;
	virtual void OnRecycleToPool() override;
	virtual void InitializeSpawnedObject(int32 InLevel, AActor* InTargetPlayer) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "OB|ObjectPool", meta = (DisplayName = "On Spawn From Pool"))
	void ReceiveOnSpawnFromPool();

	UFUNCTION(BlueprintImplementableEvent, Category = "OB|ObjectPool", meta = (DisplayName = "On Recycle To Pool"))
	void ReceiveOnRecycleToPool();

	UFUNCTION()
	virtual void OnItemOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> ItemMesh;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> TargetPlayer = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS|SetByCaller")
	FGameplayTag SetByCallerTag;
};
