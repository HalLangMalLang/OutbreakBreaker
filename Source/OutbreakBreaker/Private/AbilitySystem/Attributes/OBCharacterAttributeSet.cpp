#include "AbilitySystem/Attributes/OBCharacterAttributeSet.h"
#include "GameplayEffectExtension.h"

void UOBCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UOBCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	if (Data.EvaluatedData.Attribute == GetXPAttribute())
	{
		if (GetXP() >= GetMaxXP())
		{
			float ExcessXP = GetXP() - GetMaxXP();

			SetLevel(GetLevel() + 1.0f);
			SetXP(ExcessXP);

			if (OnLevelUpDelegate.IsBound())
			{
				HandleLevelUpNotification(GetLevel());
			}
		}
	}
}

void UOBCharacterAttributeSet::HandleDeathNotification(AActor* Destroyer)
{
	OnCharacterDeadDelegate.Broadcast(Destroyer);
}

void UOBCharacterAttributeSet::HandleLevelUpNotification(float NewLevle)
{
	OnLevelUpDelegate.Broadcast(NewLevle);
}

void UOBCharacterAttributeSet::HandleSpeedChangedNotification(float NewSpeed)
{
	OnSpeedChangedDelegate.Broadcast(NewSpeed);
}