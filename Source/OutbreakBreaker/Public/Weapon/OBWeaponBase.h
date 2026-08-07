#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OBWeaponBase.generated.h"

class AOBCharacter;

UENUM(BlueprintType)
enum class EOBWeaponState : uint8
{
	None,
	Selected,  // 선택 상태 : 장착되어 플레이어가 직접 사격/조종하는 상태
	Passive    // 대기 상태 : 캐릭터 주변에서 자동 공격/방어하는 상태
};

UCLASS()
class OUTBREAKBREAKER_API AOBWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	AOBWeaponBase();

	void SetTargetAnchor(USceneComponent* InAnchor);
	virtual void SetWeaponState(EOBWeaponState NewState);

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon | State")
	EOBWeaponState CurrentWeaponState = EOBWeaponState::Passive;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Follow")
	float FollowSpeed = 6.0f;
};
