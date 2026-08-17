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
	Max                    UMETA(Hidden) 
};

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