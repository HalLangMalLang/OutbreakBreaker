#include "Character/OBCharacterBase.h"
#include "AbilitySystem/OBAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


AOBCharacterBase::AOBCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

UAbilitySystemComponent* AOBCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AOBCharacterBase::GetAttributeSet() const
{
	return AttributeSet;
}

void AOBCharacterBase::ModifyGameplayTag(FGameplayTag Tag, bool bAdd)
{
	if (!Tag.IsValid())
	{
		return;
	}

	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!ASC)
	{
		return;
	}

	if (bAdd)
	{
		ASC->AddLooseGameplayTag(Tag);
	}
	else
	{
		ASC->RemoveLooseGameplayTag(Tag);
	}
}

void AOBCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);

	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void AOBCharacterBase::AddCharacterAbilities()
{
	if (UOBAbilitySystemComponent* OBAbilitySystemComponent = Cast<UOBAbilitySystemComponent>(AbilitySystemComponent))
	{
		OBAbilitySystemComponent->AddCharacterAbilities(StartupAbilities);
	}
}

void AOBCharacterBase::InitializeDefaultAttributes()
{
	ApplyEffectToSelf(InitAttributes, 1.f);
	ApplyEffectToSelf(FillAttributes, 1.f);
}

void AOBCharacterBase::InitializeAttributeDelegates() {}

void AOBCharacterBase::InitAbilityActorInfo() {}

void AOBCharacterBase::OnLevelUpProcessed(float NewLevel) {}

void AOBCharacterBase::OnCharacterDeathProcessed(AActor* Destroyer) {}

void AOBCharacterBase::OnMoveSpeedVelocityUpdated(float NewSpeed)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
	}
}