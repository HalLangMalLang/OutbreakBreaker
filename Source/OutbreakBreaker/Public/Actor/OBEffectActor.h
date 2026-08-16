#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "OBEffectActor.generated.h"

class UGameplayEffect;
class UAuraAbilitySystemComponent;

UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

UENUM(BlueprintType)
enum class EEffectRemovalPolicy : uint8
{
	RemoveOnEndOverlap,
	DoNotApply
};

UCLASS()
class OUTBREAKBREAKER_API AOBEffectActor : public AActor
{
	GENERATED_BODY()

public:
	AOBEffectActor();

	void SetActorLevel(float NewLevel);

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "GAS|Effect")
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass, AActor* SourceActor = nullptr, FGameplayTag DataTag = FGameplayTag(), float Magnitude = 0.0f);

	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor, AActor* SourceActor = nullptr, FGameplayTag DataTag = FGameplayTag(), float Magnitude = 0.0f);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor, AActor* SourceActor = nullptr, FGameplayTag DataTag = FGameplayTag(), float Magnitude = 0.0f);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	bool bDestroyOnEffectRemoval = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect>  InstantGameplayEffectClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectApplicationPolicy InstantEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect>  DurationGameplayEffectClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectApplicationPolicy DurationEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect>  InfiniteGameplayEffectClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectRemovalPolicy InfiniteEffectRemovalPolicy = EEffectRemovalPolicy::RemoveOnEndOverlap;

	UPROPERTY(VisibleDefaultsOnly)
	float EffectMagnitude = 0;

private:
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;

	UPROPERTY(EditAnywhere, Category = "Applied Effects")
	float ActorLevel = 1.f;
};
