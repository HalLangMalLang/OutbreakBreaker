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

	TArray<FOBUpgradeCardData> TempWeaponPool = WeaponCardPool;
	TArray<FOBUpgradeCardData> TempBuffPool = BuffCardPool;

	// 5레벨, 10레벨, 15레벨 등 5의 배수 레벨
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
			if (TotalRemaining == 0) break;

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
	if (!UpgradeCardDataTable) return;

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

void UOBUpgradeComponent::ApplyUpgradeCard(const FOBUpgradeCardData& SelectedCard)
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

	if (SelectedCard.UpgradeGameplayEffect && SelectedCard.EffectTag.IsValid())
	{
		UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(PlayerChar);
		if (ASC)
		{
			FGameplayEffectContextHandle Context = ASC->MakeEffectContext();

			FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(SelectedCard.UpgradeGameplayEffect, 1.0f, Context);

			if (SpecHandle.IsValid())
			{
				FGameplayEffectSpec* Spec = SpecHandle.Data.Get();
				Spec->SetSetByCallerMagnitude(SelectedCard.EffectTag, SelectedCard.Value);
				ASC->ApplyGameplayEffectSpecToSelf(*Spec);
			}
		}
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
