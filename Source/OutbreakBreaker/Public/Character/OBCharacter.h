#pragma once

#include "CoreMinimal.h"
#include "Character/OBCharacterBase.h"
#include "GameplayTagContainer.h"
#include "OBCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
struct FOBWeaponInfoRow;
class UOBInputConfig;
class UGameplayEffect;

UCLASS()
class OUTBREAKBREAKER_API AOBCharacter : public AOBCharacterBase
{
	GENERATED_BODY()

public:
	AOBCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void NotifyControllerChanged() override;

	void DodgingLaunch();
	void StopDodgingMovement();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon | System")
	AActor* GetWeaponFromMap(FGameplayTag WeaponTag) const;

	UFUNCTION(BlueprintCallable, Category = "Weapon | System")
	void RegisterWeaponToMap(FGameplayTag WeaponTag, AActor* NewWeapon);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() override;
	virtual void InitializeAttributeDelegates() override;

	virtual void OnLevelUpProcessed(float NewLevel) override;
	virtual void OnCharacterDeathProcessed(AActor* Destroyer) override;

	void Move(const FInputActionValue& Value);
	// 마우스 커서 방향으로 캐릭터를 부드럽게 회전시키는 함수
	void RotateToCursor(float DeltaTime);
	// 탑다운 게임을 위한 마우스 커서 및 입력 모드를 설정하는 함수
	void SetupMouseInterface();
	FVector GetDodgeDirection() const;
	void UpdateDodgePosition(float DeltaTime);

	void ApplyWeaponEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, const FOBWeaponInfoRow& WeaponInfo);

private:
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UOBInputConfig> InputConfig;

	// 마우스 커서를 바라보는 회전 보간 속도 (높을수록 빠르게 반응, 10.0 ~ 20.0 추천)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Movement")
	float RotationInterpSpeed = 15.0f;

	float DodgeElapsedTime = 0.f;
	FVector DodgeStartLocation;
	FVector DodgeDirection;

	UPROPERTY(EditAnywhere)
	float DodgeDuration = 0.75f;
	UPROPERTY(EditAnywhere)
	float DodgeDistance = 100.f;
	bool bIsDodging = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS | Data")
	TObjectPtr<UDataTable> WeaponMasterDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS | Data")
	TSubclassOf<UGameplayEffect> WeaponAttributesInitialize;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon | System")
	TMap<FGameplayTag, AActor*> WeaponMap;

private:
	UPROPERTY(EditDefaultsOnly, Category = "GAS | Progression")
	TSubclassOf<UGameplayEffect> RefreshMaxXPClass;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TMap<FGameplayTag, FGameplayTag> InputToWeaponTagMap;
};
