#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DataStructures/OBWeaponDataStructures.h"
#include "OBUpgradeComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class OUTBREAKBREAKER_API UOBUpgradeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UOBUpgradeComponent();

	UFUNCTION(BlueprintCallable, Category = "Upgrade | Manager")
	TArray<FOBUpgradeCardData> GetRandomUpgradeCards(int32 CurrentPlayerLevel, int32 CardCount = 3);

	UFUNCTION(BlueprintCallable, Category = "Upgrade")
	void ApplyUpgradeCard(const FOBUpgradeCardData& SelectedCard, int32 InCardStage);

	UFUNCTION(BlueprintCallable, Category = "Upgrade | Start")
	TArray<FOBUpgradeCardData> GetStartWeaponCards();

protected:
	virtual void BeginPlay() override;

	void InitializeUpgradeCardPool();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Upgrade | Setup")
	TObjectPtr<UDataTable> UpgradeCardDataTable;

private:
	UPROPERTY()
	TArray<FOBUpgradeCardData> WeaponCardPool;

	UPROPERTY()
	TArray<FOBUpgradeCardData> BuffCardPool;
};
