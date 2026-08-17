#include "ActorComponent/OBWeaponComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/Attributes/OBWeaponAttributeSetBase.h"
#include "AbilitySystem/Attributes/OBCharacterAttributeSet.h"

UOBWeaponComponent::UOBWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UOBWeaponComponent::InitializeOrUpgradeWeapon(EWeaponType InWeaponType, int32 InNewLevel)
{
	AActor* OwnerActor = GetOwner();
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerActor);
	if (!ASC || !WeaponAssetMap.Contains(InWeaponType))
	{
		return;
	}

	FOBWeaponAssetPreset& TargetPreset = WeaponAssetMap[InWeaponType];
	if (!TargetPreset.WeaponDataTable || !TargetPreset.LevelUpgradeGE)
	{
		return;
	}

	if (TargetPreset.bIsInitialized == false)
	{
		if (TargetPreset.InitializationGE)
		{
			FGameplayEffectContextHandle InitContext = ASC->MakeEffectContext();
			FGameplayEffectSpecHandle InitSpecHandle = ASC->MakeOutgoingSpec(TargetPreset.InitializationGE, 1.0f, InitContext);
			if (InitSpecHandle.IsValid())
			{
				ASC->ApplyGameplayEffectSpecToSelf(*InitSpecHandle.Data.Get());
			}
		}
		TargetPreset.bIsInitialized = true;
	}

	FName RowName = *FString::Printf(TEXT("Level%d"), InNewLevel);
	FOBWeaponLevelData* LevelData = TargetPreset.WeaponDataTable->FindRow<FOBWeaponLevelData>(RowName, TEXT(""));
	if (!LevelData)
	{
		return;
	}

	FGameplayEffectContextHandle UpgradeContext = ASC->MakeEffectContext();
	FGameplayEffectSpecHandle LevelSpecHandle = ASC->MakeOutgoingSpec(TargetPreset.LevelUpgradeGE, 1.0f, UpgradeContext);

	if (LevelSpecHandle.IsValid())
	{
		FGameplayEffectSpec* Spec = LevelSpecHandle.Data.Get();

		Spec->SetSetByCallerMagnitude(LevelData->PassiveDamage.AttributeTag, LevelData->PassiveDamage.AttributeValue);
		Spec->SetSetByCallerMagnitude(LevelData->PassiveAttackDelay.AttributeTag, LevelData->PassiveAttackDelay.AttributeValue);
		Spec->SetSetByCallerMagnitude(LevelData->SelectedDamage.AttributeTag, LevelData->SelectedDamage.AttributeValue);
		Spec->SetSetByCallerMagnitude(LevelData->SelectedAttackDelay.AttributeTag, LevelData->SelectedAttackDelay.AttributeValue);
		Spec->SetSetByCallerMagnitude(LevelData->CommonAttackSpeed.AttributeTag, LevelData->CommonAttackSpeed.AttributeValue);

		for (const FOBWeaponAttributePair& AttrPair : LevelData->SpecialAttributes)
		{
			if (AttrPair.AttributeTag.IsValid())
			{
				Spec->SetSetByCallerMagnitude(AttrPair.AttributeTag, AttrPair.AttributeValue);
			}
		}
		ASC->ApplyGameplayEffectSpecToSelf(*Spec);
	}
}

float UOBWeaponComponent::GetFinalAttributeValue(FGameplayAttribute InAttribute) const
{
	if (!InAttribute.IsValid())
	{
		return 0.f;
	}

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	if (!ASC)
	{
		return 0.f;
	}

	const UOBWeaponAttributeSetBase* WeaponSet = Cast<UOBWeaponAttributeSetBase>(ASC->GetAttributeSet(UOBWeaponAttributeSetBase::StaticClass()));
	const UOBCharacterAttributeSet* CharSet = Cast<UOBCharacterAttributeSet>(ASC->GetAttributeSet(UOBCharacterAttributeSet::StaticClass()));
	if (!WeaponSet)
	{
		return 0.f;
	}

	float RawValue = InAttribute.GetNumericValue(WeaponSet);


	float BaseBonus = WeaponSet->GetBonusDamage();
	float WeaponMultiplier = 1.0f;
	float CharacterMultiplier = 1.0f;

	FString AttribName = InAttribute.GetName();

	if (AttribName.Contains(TEXT("Damage")))
	{
		RawValue += WeaponSet->GetBonusDamage();
		WeaponMultiplier = WeaponSet->GetWeaponDamageMultiplier();
		CharacterMultiplier = CharSet ? CharSet->GetAttackPowerMultiplier() : 1.0f;

		return RawValue * WeaponMultiplier * CharacterMultiplier;
	}

	if (AttribName.Contains(TEXT("AttackDelay")))
	{
		RawValue = FMath::Max(0.05f, RawValue - WeaponSet->GetBonusAttackDelay());
		WeaponMultiplier = WeaponSet->GetWeaponAttackSpeedMultiplier();
		CharacterMultiplier = CharSet ? CharSet->GetAttackSpeedMultiplier() : 1.0f;

		return RawValue / (WeaponMultiplier * CharacterMultiplier * WeaponSet->GetCommonAttackSpeed());
	}

	if (AttribName.Contains(TEXT("Range")) || AttribName.Contains(TEXT("Radius")))
	{
		RawValue += WeaponSet->GetBonusRange();
		WeaponMultiplier = WeaponSet->GetWeaponRangeMultiplier();

		return RawValue * WeaponMultiplier;
	}

	return RawValue;
}

