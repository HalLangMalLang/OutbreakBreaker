#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Abilities/GameplayAbility.h"
#include "OBWeaponBase.generated.h"

class AOBCharacter;
class UOBWeaponAttributeSetBase;

UCLASS()
class OUTBREAKBREAKER_API AOBWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	AOBWeaponBase();

	UFUNCTION(BlueprintCallable)
	void SetTargetAnchor(USceneComponent* InAnchor);
	UFUNCTION(BlueprintCallable)
	void ToggleWeaponMode(bool bIsActiveMode);

	FORCEINLINE FGameplayTag GetActiveTag() const { return ActiveTag; }
	FORCEINLINE FGameplayTag GetPassiveTag() const { return PassiveTag; }
	FORCEINLINE FGameplayTag GetCurrentTag() const { return CurrentTag; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetOwner(AActor* NewOwner) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon | Components")
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon | Follow")
	TObjectPtr<USceneComponent> TargetAnchor;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon | Owner")
	TObjectPtr<AOBCharacter> OwnerCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon | GAS")
	TWeakObjectPtr<UOBWeaponAttributeSetBase> AttributeSet;

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
