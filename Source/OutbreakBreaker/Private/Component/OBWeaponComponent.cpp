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

	// InAttribute가 속한 '실제 자식 AttributeSet 인스턴스'를 ASC에서 동적으로 찾아옵니다.
	// 상속 관계 덕분에 베이스 클래스 포인터 하나로 자식 세트(SwordSet, GunSet 등)를 다 받아낼 수 있습니다.
	const UOBWeaponAttributeSetBase* WeaponSet = Cast<UOBWeaponAttributeSetBase>(ASC->GetAttributeSet(InAttribute.GetAttributeSetClass()));
	const UOBCharacterAttributeSet* CharSet = Cast<UOBCharacterAttributeSet>(ASC->GetAttributeSet(UOBCharacterAttributeSet::StaticClass()));

	if (!WeaponSet)
	{
		return 0.f;
	}

	// 해당 무기 전용 주머니에서 순수 데이터 테이블 적용값(RawValue) 추출
	float RawValue = InAttribute.GetNumericValue(WeaponSet);

	// [상속 대응 완료] 문자열 Contains 대신 정적 속성 이름 매칭
	// 자식 클래스에서 상속받은 속성이어도 이름이 "PassiveDamage"나 "SelectedDamage"이면 완벽히 일치 판정이 납니다.
	const FName AttribName = InAttribute.GetUProperty()->GetFName();

	// --- [ 데미지 계열 계산 ] ---
	if (AttribName == GET_MEMBER_NAME_CHECKED(UOBWeaponAttributeSetBase, PassiveDamage) ||
		AttribName == GET_MEMBER_NAME_CHECKED(UOBWeaponAttributeSetBase, SelectedDamage))
	{
		RawValue += WeaponSet->GetBonusDamage();
		float WeaponMultiplier = WeaponSet->GetWeaponDamageMultiplier();
		float CharacterMultiplier = CharSet ? CharSet->GetAttackPowerMultiplier() : 1.0f;

		return RawValue * WeaponMultiplier * CharacterMultiplier;
	}

	// --- [ 공격 딜레이/속도 계열 계산 ] ---
	if (AttribName == GET_MEMBER_NAME_CHECKED(UOBWeaponAttributeSetBase, PassiveAttackDelay) ||
		AttribName == GET_MEMBER_NAME_CHECKED(UOBWeaponAttributeSetBase, SelectedAttackDelay))
	{
		RawValue = FMath::Max(0.05f, RawValue - WeaponSet->GetBonusAttackDelay());
		float WeaponMultiplier = WeaponSet->GetWeaponAttackSpeedMultiplier();
		float CharacterMultiplier = CharSet ? CharSet->GetAttackSpeedMultiplier() : 1.0f;
		float CommonSpeed = WeaponSet->GetCommonAttackSpeed();

		// Zero Division(0으로 나누기) 크래시 방지용 안전장치
		float TotalMultiplier = FMath::Max(0.01f, WeaponMultiplier * CharacterMultiplier * CommonSpeed);

		return RawValue / TotalMultiplier;
	}

	// --- [ 사거리/범위 계열 계산 ] ---
	if (AttribName == GET_MEMBER_NAME_CHECKED(UOBWeaponAttributeSetBase, PassiveAttackRange))
	{
		RawValue += WeaponSet->GetBonusRange();
		float WeaponMultiplier = WeaponSet->GetWeaponRangeMultiplier();

		return RawValue * WeaponMultiplier;
	}

	// 곱연산 주머니(Multiplier)나 레벨(Level) 자체를 요청한 경우 연산 없이 순수 값 반환
	return RawValue;
}


void UOBWeaponComponent::DeinitializeWeapon(EWeaponType InWeaponType)
{
	if (FOBWeaponAssetPreset* FoundPreset = WeaponAssetMap.Find(InWeaponType))
	{
		FoundPreset->bIsInitialized = false;

		UE_LOG(LogTemp, Log, TEXT("%d 타입 무기가 반납되어 초기화 상태가 해제되었습니다."), (int32)InWeaponType);
	}
}

