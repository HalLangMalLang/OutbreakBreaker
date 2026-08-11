#include "AbilitySystem/Attributes/OBCharacterAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "AbilitySystemBlueprintLibrary.h"

void UOBCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHPAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHP());
	}
}

void UOBCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	if (Data.EvaluatedData.Attribute == GetHPAttribute())
	{
		SetHP(FMath::Clamp(GetHP(), 0.f, GetMaxHP()));
	}

	if (Data.EvaluatedData.Attribute == GetXPAttribute())
	{
		if (GetXP() >= GetMaxXP())
		{
			float ExcessXP = GetXP() - GetMaxXP();
			SetLevel(GetLevel() + 1.0f);
			SetXP(ExcessXP);

			if (OnLevelUpDelegate.IsBound())
			{
				OnLevelUpDelegate.Broadcast(GetLevel());
			}
		}
	}

	if (Data.EvaluatedData.Attribute == GetHPAttribute())
	{
		if (GetHP() <= 0.f)
		{
			if (OnCharacterDeadDelegate.IsBound())
			{
				OnCharacterDeadDelegate.Broadcast(Props.SourceAvatarActor);
			}
		}
	}

	if (Data.EvaluatedData.Attribute == GetMoveSpeedAttribute())
	{
		if (OnSpeedChangedDelegate.IsBound())
		{
			OnSpeedChangedDelegate.Broadcast(GetMoveSpeed());
		}
	}
}

void UOBCharacterAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	// Context에서 공격자/피해자 정보 추출
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		// Source = 공격자
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}

		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}

	// Target = 피해자
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}
