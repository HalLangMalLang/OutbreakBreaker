#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "DataStructures/OBWeaponDataStructures.h"
#include "OBWeaponComponent.generated.h"

class UDataTable;
class UGameplayEffect;
struct FGameplayAttribute;

USTRUCT(BlueprintType)
struct FOBWeaponAssetPreset
{
	GENERATED_BODY()

public:
	// 무기 레벨별 테이블 속성
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Asset")
	TObjectPtr<UDataTable> WeaponDataTable;

	// 최초 무기 획득 시 초기화 GE
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Asset")
	TSubclassOf<UGameplayEffect> InitializationGE;

	// 레벨 전용 GE */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Asset")
	TSubclassOf<UGameplayEffect> LevelUpgradeGE;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Runtime")
	bool bIsInitialized = false;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class OUTBREAKBREAKER_API UOBWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UOBWeaponComponent();

	UFUNCTION(BlueprintCallable)
	void InitializeOrUpgradeWeapon(EWeaponType InWeaponType, int32 InNewLevel);

	float GetFinalAttributeValue(FGameplayAttribute InAttribute) const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Setup", meta = (AllowPrivateAccess = "true"))
	TMap<EWeaponType, FOBWeaponAssetPreset> WeaponAssetMap;
};
