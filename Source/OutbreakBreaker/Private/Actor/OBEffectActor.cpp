#include "Actor/OBEffectActor.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/OBAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

AOBEffectActor::AOBEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AOBEffectActor::SetActorLevel(float NewLevel)
{
	ActorLevel = NewLevel;
}

void AOBEffectActor::BeginPlay()
{
	Super::BeginPlay();

}

void AOBEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass, AActor* SourceActor, FGameplayTag DataTag, float Magnitude)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetASC)
	{
		return;
	}

	check(GameplayEffectClass);

	FGameplayEffectContextHandle EffectContextHandle;
	AActor* FinalSource = nullptr;

	if (SourceActor != nullptr)
	{
		UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
		EffectContextHandle = SourceASC ? SourceASC->MakeEffectContext() : TargetASC->MakeEffectContext();

		FinalSource = SourceActor;
	}
	else
	{
		EffectContextHandle = TargetASC->MakeEffectContext();

		FinalSource = this;
	}

	EffectContextHandle.AddSourceObject(FinalSource);

	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);

	if (DataTag.IsValid())
	{
		EffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(DataTag, Magnitude);
	}

	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
	}
}

void AOBEffectActor::OnOverlap(AActor* TargetActor, AActor* SourceActor, FGameplayTag DataTag, float Magnitude)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass, SourceActor, DataTag, Magnitude);
	}

	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass, SourceActor, DataTag, Magnitude);
	}

	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass, SourceActor, DataTag, Magnitude);
	}
}

void AOBEffectActor::OnEndOverlap(AActor* TargetActor, AActor* SourceActor, FGameplayTag DataTag, float Magnitude)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass, SourceActor, DataTag, Magnitude);
	}

	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass, SourceActor, DataTag, Magnitude);
	}

	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass, SourceActor, DataTag, Magnitude);
	}

	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

		if (!IsValid(TargetASC))
		{
			return;
		}

		TArray<FActiveGameplayEffectHandle> HandlesToRemove;

		for (TTuple<FActiveGameplayEffectHandle, UAbilitySystemComponent*> HandlePair : ActiveEffectHandles)
		{
			if (TargetASC == HandlePair.Value)
			{
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key);
				HandlesToRemove.Add(HandlePair.Key);
			}
		}

		for (auto& Handle : HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}
}

