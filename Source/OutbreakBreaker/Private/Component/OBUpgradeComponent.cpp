#include "Component/OBUpgradeComponent.h"
#include "Component/OBWeaponComponent.h" 
#include "AbilitySystemBlueprintLibrary.h"
#include "Character/OBCharacter.h"
#include "AbilitySystemComponent.h"

UOBUpgradeComponent::UOBUpgradeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

TArray<FOBUpgradeCardData> UOBUpgradeComponent::GetRandomUpgradeCards(int32 CurrentPlayerLevel, int32 CardCount)
{
	TArray<FOBUpgradeCardData> OutSelectedCards;

	if (WeaponCardPool.Num() == 0 && BuffCardPool.Num() == 0)
	{
		return OutSelectedCards;
	}

	AOBCharacter* PlayerChar = Cast<AOBCharacter>(GetOwner());
	UOBWeaponComponent* WeaponComp = PlayerChar ? PlayerChar->GetWeaponComponent() : nullptr;
	if (!WeaponComp)
	{
		return OutSelectedCards;
	}

	// 플레이어가 진짜로 들고 있는(레벨 1이상) 무기 이름 목록 수집
	TArray<FString> RealOwnedWeaponNames;
	for (EWeaponType Type : TEnumRange<EWeaponType>())
	{
		if (WeaponComp->GetCurrentWeaponLevel(Type) >= 1)
		{
			FString NameStr = UEnum::GetValueAsString(Type);
			NameStr.Split(TEXT("::"), nullptr, &NameStr);
			RealOwnedWeaponNames.Add(NameStr);
		}
	}

	//  무기 공용 증강 카드 중, 플레이어가 장착하지도 않은 무기 전용 태그만 들어있는 카드는 후보 풀에서 원천 배제
	TArray<FOBUpgradeCardData> TempWeaponPool;
	for (const FOBUpgradeCardData& Card : WeaponCardPool)
	{
		// 특정 무기 자체를 해금/레벨업하는 카드는 당연히 언제나 등장할 수 있으므로 패스
		if (Card.TargetWeaponType != EWeaponType::None)
		{
			TempWeaponPool.Add(Card);
			continue;
		}

		// 무기 공용 증강 카드의 경우, 내가 들고 있는 무기 태그가 최소 1개 이상 겹치는지 검사
		bool bHasValidWeaponTag = false;
		for (const FGameplayTag& Tag : Card.EffectTags)
		{
			for (const FString& OwnedName : RealOwnedWeaponNames)
			{
				if (Tag.ToString().Contains(OwnedName))
				{
					bHasValidWeaponTag = true;
					break;
				}
			}

			if (bHasValidWeaponTag)
			{
				break;
			}
		}

		// 내가 현재 장착한 무기 스탯을 바꿀 수 있는 '유효한' 카드만 무기 뽑기 통에 넣습니다.
		if (bHasValidWeaponTag)
		{
			TempWeaponPool.Add(Card);
		}
	}

	TArray<FOBUpgradeCardData> TempBuffPool = BuffCardPool;

	// 5레벨, 10레벨, 15레벨 등 5의 배수 레벨 무기 확정 조건 체크
	bool bIsWeaponGuaranteedLevel = (CurrentPlayerLevel > 0 && CurrentPlayerLevel % 5 == 0);

	if (TempWeaponPool.Num() == 0)
	{
		bIsWeaponGuaranteedLevel = false;
	}

	int32 CardsToDraw = FMath::Min(CardCount, TempWeaponPool.Num() + TempBuffPool.Num());

	for (int32 i = 0; i < CardsToDraw; ++i)
	{
		if (i == 0 && bIsWeaponGuaranteedLevel)
		{
			int32 RandIdx = FMath::RandRange(0, TempWeaponPool.Num() - 1);
			OutSelectedCards.Add(TempWeaponPool[RandIdx]);
			TempWeaponPool.RemoveAt(RandIdx);
		}
		else
		{
			int32 TotalRemaining = TempWeaponPool.Num() + TempBuffPool.Num();
			if (TotalRemaining == 0)
			{
				break;
			}

			int32 RandRoll = FMath::RandRange(0, TotalRemaining - 1);

			if (RandRoll < TempWeaponPool.Num())
			{
				OutSelectedCards.Add(TempWeaponPool[RandRoll]);
				TempWeaponPool.RemoveAt(RandRoll);
			}
			else
			{
				int32 BuffIdx = RandRoll - TempWeaponPool.Num();
				OutSelectedCards.Add(TempBuffPool[BuffIdx]);
				TempBuffPool.RemoveAt(BuffIdx);
			}
		}
	}

	return OutSelectedCards;
}

void UOBUpgradeComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeUpgradeCardPool();
}

void UOBUpgradeComponent::InitializeUpgradeCardPool()
{
	if (!UpgradeCardDataTable)
	{
		return;
	}

	WeaponCardPool.Empty();
	BuffCardPool.Empty();

	TArray<FOBUpgradeCardData*> AllRows;
	UpgradeCardDataTable->GetAllRows<FOBUpgradeCardData>(TEXT(""), AllRows);

	for (const FOBUpgradeCardData* RowPtr : AllRows)
	{
		if (!RowPtr)
		{
			continue;
		}

		if (RowPtr->TargetWeaponType != EWeaponType::None)
		{
			WeaponCardPool.Add(*RowPtr);
		}
		else
		{
			BuffCardPool.Add(*RowPtr);
		}
	}
}

void UOBUpgradeComponent::ApplyUpgradeCard(const FOBUpgradeCardData& SelectedCard, int32 InCardStage)
{
	AOBCharacter* PlayerChar = Cast<AOBCharacter>(GetOwner());
	if (!PlayerChar)
	{
		return;
	}

	if (SelectedCard.TargetWeaponType != EWeaponType::None)
	{
		UOBWeaponComponent* WeaponComp = PlayerChar->GetWeaponComponent();
		if (WeaponComp)
		{
			int32 NextLevel = WeaponComp->GetCurrentWeaponLevel(SelectedCard.TargetWeaponType) + 1;
			WeaponComp->InitializeOrUpgradeWeapon(SelectedCard.TargetWeaponType, NextLevel);
		}

		return;
	}

	if (!SelectedCard.UpgradeGameplayEffect || SelectedCard.EffectTags.IsEmpty())
	{
		return;
	}

	UOBWeaponComponent* WeaponComp = PlayerChar->GetWeaponComponent();
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(PlayerChar);
	if (!WeaponComp || !ASC)
	{
		return;
	}

	TArray<EWeaponType> RealOwnedWeapons;
	for (EWeaponType Type : TEnumRange<EWeaponType>())
	{
		if (WeaponComp->GetCurrentWeaponLevel(Type) >= 1)
		{
			RealOwnedWeapons.Add(Type);
		}
	}

	TArray<FGameplayTag> ValidCandidateTags;

	for (const FGameplayTag& Tag : SelectedCard.EffectTags)
	{
		if (!Tag.IsValid())
		{
			continue;
		}

		if (Tag.ToString().Contains(TEXT("Character.Stat")))
		{
			ValidCandidateTags.Add(Tag);
			continue;
		}

		for (EWeaponType OwnedType : RealOwnedWeapons)
		{
			FString WeaponNameStr = UEnum::GetValueAsString(OwnedType);
			WeaponNameStr.Split(TEXT("::"), nullptr,
				&WeaponNameStr); 

			if (Tag.ToString().Contains(WeaponNameStr))
			{
				ValidCandidateTags.Add(Tag);
				break;
			}
		}
	}

	if (ValidCandidateTags.IsEmpty())
	{
		return;
	}

	int32 RandomIndex = FMath::RandRange(0, ValidCandidateTags.Num() - 1);
	FGameplayTag FinalChosenTag = ValidCandidateTags[RandomIndex];

	FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(SelectedCard.UpgradeGameplayEffect, 1.0f, Context);

	if (SpecHandle.IsValid() && FinalChosenTag.IsValid())
	{
		FGameplayEffectSpec* Spec = SpecHandle.Data.Get();

		float StatValue = SelectedCard.StageValues.IsValidIndex(InCardStage) ? SelectedCard.StageValues[InCardStage] : 0.0f;

		Spec->SetSetByCallerMagnitude(FinalChosenTag, StatValue);
		ASC->ApplyGameplayEffectSpecToSelf(*Spec);

		UE_LOG(LogTemp, Log, TEXT("[최종 증강 성공] 당첨 태그: %s (적용 수치: %f)"), *FinalChosenTag.ToString(), StatValue);
	}
}

TArray<FOBUpgradeCardData> UOBUpgradeComponent::GetStartWeaponCards()
{
	TArray<FOBUpgradeCardData> MainWeaponOnlyPool;

	if (WeaponCardPool.Num() == 0)
	{
		return MainWeaponOnlyPool;
	}

	for (const FOBUpgradeCardData& Card : WeaponCardPool)
	{
		if (Card.TargetWeaponType == EWeaponType::PlasmaRifle ||
			Card.TargetWeaponType == EWeaponType::GravityHammer ||
			Card.TargetWeaponType == EWeaponType::SpreadShotgun)
		{
			bool bAlreadyAdded = MainWeaponOnlyPool.ContainsByPredicate([&Card](const FOBUpgradeCardData& AddedCard) {
				return AddedCard.TargetWeaponType == Card.TargetWeaponType;
				});

			if (!bAlreadyAdded)
			{
				MainWeaponOnlyPool.Add(Card);
			}
		}
	}

	return MainWeaponOnlyPool;
}
