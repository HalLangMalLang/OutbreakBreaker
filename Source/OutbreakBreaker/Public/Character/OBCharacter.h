#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "OBCharacter.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
struct FOBWeaponInfoRow;
class UOBInputConfig;
class UGameplayAbility;
class UGameplayEffect;
class UOBPlasmaRifleAttributeSet;
class UOBGravityHammerAttributeSet;
class UOBSpreadShotgunAttributeSet;
class UOBDefenseDroneAttributeSet;
class UOBMagnetMineAttributeSet;
class UOBAuraModuleAttributeSet;

UCLASS()
class OUTBREAKBREAKER_API AOBCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AOBCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void NotifyControllerChanged() override;

	void DodgingLaunch();
	void StopDodgingMovement();

	UFUNCTION(BlueprintCallable)
	void SetWeaponTag(FGameplayTag Tag, bool bAdd);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon | System")
	AActor* GetWeaponFromMap(FGameplayTag WeaponTag) const;
	UFUNCTION(BlueprintCallable, Category = "Weapon | System")
	void RegisterWeaponToMap(FGameplayTag WeaponTag, AActor* NewWeapon);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void Move(const FInputActionValue& Value);

	// 마우스 커서 방향으로 캐릭터를 부드럽게 회전시키는 함수
	void RotateToCursor(float DeltaTime);

	// 탑다운 게임을 위한 마우스 커서 및 입력 모드를 설정하는 함수
	void SetupMouseInterface();

	void InitAbilityActorInfo();

	void AddCharacterAbilities();

	FVector GetDodgeDirection() const;

	void UpdateDodgePosition(float DeltaTime);

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;
	void ApplyWeaponEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, const FOBWeaponInfoRow& WeaponInfo);

	void InitializeDefaultAttributes();

private:
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> CharacterAttributeSet;

	/** 주무기 3종 속성 세트 캐싱 포인터 */
	UPROPERTY()
	TObjectPtr<UOBPlasmaRifleAttributeSet> PlasmaRifleAttributes;
	UPROPERTY()
	TObjectPtr<UOBGravityHammerAttributeSet> GravityHammerAttributes;
	UPROPERTY()
	TObjectPtr<UOBSpreadShotgunAttributeSet> SpreadShotgunAttributes;

	/** 보조무기 3종 속성 세트 캐싱 포인터 */
	UPROPERTY()
	TObjectPtr<UOBDefenseDroneAttributeSet> DefenseDroneAttributes;
	UPROPERTY()
	TObjectPtr<UOBMagnetMineAttributeSet> MagnetMineAttributes;
	UPROPERTY()
	TObjectPtr<UOBAuraModuleAttributeSet> AuraModuleAttributes;

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

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> InitAttributes;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> FillAttributes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS | Data")
	TObjectPtr<UDataTable> WeaponMasterDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS | Data")
	TSubclassOf<UGameplayEffect> WeaponAttributesInitialize;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon | System")
	TMap<FGameplayTag, AActor*> WeaponMap;

private:
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
};
