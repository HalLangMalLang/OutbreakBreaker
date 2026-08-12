#include "AbilitySystem/Attributes/OBEnemyAttributeSet.h"

void UOBEnemyAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UOBEnemyAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}

void UOBEnemyAttributeSet::HandleDeathNotification(AActor* Destroyer)
{
	OnEnemyDeadDelegate.Broadcast(Destroyer);
}

void UOBEnemyAttributeSet::HandleSpeedChangedNotification(float NewSpeed)
{
	OnSpeedEnemyDelegate.Broadcast(NewSpeed);
}