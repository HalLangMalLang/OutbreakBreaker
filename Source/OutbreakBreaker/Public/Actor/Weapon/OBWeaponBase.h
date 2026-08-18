#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Abilities/GameplayAbility.h"
#include "Core/PoolableInterface.h"
#include "Interface/OBSpawnableInterface.h"
#include "DataStructures/OBWeaponDataStructures.h"
#include "OBWeaponBase.generated.h"

class AOBCharacter;
class UOBWeaponAttributeSetBase;
class UOBWeaponComponent;

UCLASS()
class OUTBREAKBREAKER_API AOBWeaponBase : public AActor, public IPoolableInterface, public IOBSpawnableInterface
{
	GENERATED_BODY()

public:
	AOBWeaponBase();

	UFUNCTION(BlueprintCallable)
	void SetTargetAnchor(USceneComponent* InAnchor);

	FORCEINLINE FGameplayTag GetActiveTag() const { return ActiveTag; }
	FORCEINLINE FGameplayTag GetPassiveTag() const { return PassiveTag; }
	FORCEINLINE FGameplayTag GetCurrentTag() const { return CurrentTag; }

	void ToggleWeaponMode(bool bIsActiveMode);

	virtual void OnSpawnFromPool() override;
	virtual void OnRecycleToPool() override;

	virtual void InitializeSpawnedObject(int32 InLevel, AActor* InTargetPlayer) override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, Category = "Weapon|Movement")
	void UpdateWeaponMovement(float DeltaTime);

	virtual void UpdateWeaponMovement_Implementation(float DeltaTime);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon | Components")
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon | Runtime")
	TObjectPtr<UOBWeaponComponent> WeaponComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon | Follow")
	TObjectPtr<USceneComponent> TargetAnchor;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon | Owner")
	TObjectPtr<AOBCharacter> OwnerCharacter;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Weapon | Type")
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Follow")
	float FollowSpeed = 6.0f;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Tags", meta = (AllowPrivateAccess = "true"))
	FGameplayTag ActiveTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Tags", meta = (AllowPrivateAccess = "true"))
	FGameplayTag PassiveTag;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon | Tags", meta = (AllowPrivateAccess = "true"))
	FGameplayTag CurrentTag;
};
