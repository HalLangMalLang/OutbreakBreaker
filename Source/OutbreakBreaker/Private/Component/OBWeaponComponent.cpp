#include "Component/OBWeaponComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/Attributes/OBWeaponAttributeSetBase.h"
#include "AbilitySystem/Attributes/OBCharacterAttributeSet.h"
#include "Core/ObjectPoolSubsystem.h"
#include "Interface/OBSpawnableInterface.h"

UOBWeaponComponent::UOBWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	for (EWeaponType Type : TEnumRange<EWeaponType>())
	{
		OwnedWeaponLevels.Add(Type, 0);
	}
}

void UOBWeaponComponent::InitializeOrUpgradeWeapon(EWeaponType InWeaponType, int32 InNewLevel)
{
	if (InWeaponType == EWeaponType::None)
	{
		return;
	}

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

		if (UWorld* World = GetWorld())
		{
			if (UObjectPoolSubsystem* PoolSubsystem = World->GetGameInstance()->GetSubsystem<UObjectPoolSubsystem>())
			{
				if (AActor* NewWeapon = PoolSubsystem->GetPooledActor(TargetPreset.ObjectPoolTag))
				{
					NewWeapon->SetActorLocation(GetOwner()->GetActorLocation(), false, nullptr, ETeleportType::TeleportPhysics);
					if (IOBSpawnableInterface* Spawnable = Cast<IOBSpawnableInterface>(NewWeapon))
					{
						Spawnable->InitializeSpawnedObject(InNewLevel, GetOwner());
					}
				}
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
		OwnedWeaponLevels.Emplace(InWeaponType, InNewLevel);
	}
}

float UOBWeaponComponent::GetFinalAttributeValue(FGameplayAttribute InAttribute) const
{
	return 0;
}


void UOBWeaponComponent::DeinitializeWeapon(EWeaponType InWeaponType)
{
	if (FOBWeaponAssetPreset* FoundPreset = WeaponAssetMap.Find(InWeaponType))
	{
		FoundPreset->bIsInitialized = false;

		UE_LOG(LogTemp, Log, TEXT("%d 타입 무기가 반납되어 초기화 상태가 해제되었습니다."), (int32)InWeaponType);
	}
}

