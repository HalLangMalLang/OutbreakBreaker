#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "OBWeaponDataStructures.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	None = 0               UMETA(DisplayName = "None (미장착)"),
	PlasmaRifle            UMETA(DisplayName = "Plasma Rifle (플라즈마 라이플)"),
	GravityHammer          UMETA(DisplayName = "Gravity Hammer (그래비티 해머)"),
	SpreadShotgun          UMETA(DisplayName = "Spread Shotgun (스프레드 샷건)"),
	DefenseDrone           UMETA(DisplayName = "Defense Drone (디펜스 드론)"),
	MagnetMine             UMETA(DisplayName = "Magnet Mine (마그넷 지뢰)"),
	AuraModule             UMETA(DisplayName = "Aura Module (오라 모듈)"),
};
ENUM_RANGE_BY_FIRST_AND_LAST(EWeaponType, EWeaponType::PlasmaRifle, EWeaponType::AuraModule);

USTRUCT(BlueprintType)
struct FOBWeaponAttributePair
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
	FGameplayTag AttributeTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
	float AttributeValue = 0.f;
};

USTRUCT(BlueprintType)
struct OUTBREAKBREAKER_API FOBUpgradeCardData : public FTableRowBase
{
	GENERATED_BODY()

public:
	// 카드의 고유 ID
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card Data")
	FName CardID;

	// 카드이름
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card Data")
	FText CardName;

	// UI에 표시될 성능 설명
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card Data")
	FText CardDescription;

	// 카드 일러스트 이미지 에셋 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card Data")
	TObjectPtr<UTexture2D> CardIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card Action")
	EWeaponType TargetWeaponType = EWeaponType::None;

	// 버프 GE
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card Action")
	TSubclassOf<class UGameplayEffect> UpgradeGameplayEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Modifier")
	TArray<FGameplayTag> EffectTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Modifier")
	TArray<float> StageValues;
};


USTRUCT(BlueprintType)
struct FOBWeaponLevelData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Passive")
	FOBWeaponAttributePair PassiveDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Passive")
	FOBWeaponAttributePair PassiveAttackDelay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Selected")
	FOBWeaponAttributePair SelectedDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Selected")
	FOBWeaponAttributePair SelectedAttackDelay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Common")
	FOBWeaponAttributePair CommonAttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Special")
	TArray<FOBWeaponAttributePair> SpecialAttributes;
};