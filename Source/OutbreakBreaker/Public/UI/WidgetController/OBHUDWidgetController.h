#pragma once

#include "CoreMinimal.h"
#include "Core/JMWidgetControllerBase.h"
#include "Library/JMUIFunctionLibrary.h"
#include "OBHUDWidgetController.generated.h"

UENUM(BlueprintType)
enum class EHUDElement : uint8
{
	HealthBar,
	ExpBar,
	WeaponSlots,
	StageTimer,
	BossHealthBar   // 평소엔 Hide, 보스전 시작 시 Show
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHUDElementVisibilityChanged, EHUDElement, Element, bool, bIsVisible);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSignature, float, NewMaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnExpChangedSignature, float, NewExp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxExpChangedSignature, float, NewMaxExp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelChangedSignature, float, NewLevel);

UCLASS(NotBlueprintable, BlueprintType)
class OUTBREAKBREAKER_API UOBHUDWidgetController : public UJMWidgetControllerBase
{
	GENERATED_BODY()

public:
	JM_WIDGET_CONTROLLER_GETTER(UOBHUDWidgetController, EJMWidgetControllerParamsType::WithGAS)

	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitialValues() override;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowAllHUD();

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void HideAllHUD();

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ToggleAllHUD();

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowHUDElement(EHUDElement Element);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void HideHUDElement(EHUDElement Element);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ToggleHUDElement(EHUDElement Element);

	UFUNCTION(BlueprintPure, Category = "HUD")
	bool IsHUDElementVisible(EHUDElement Element) const;

protected:

private:
	void InitializeVisibility();

public:

protected:
	UPROPERTY(BlueprintAssignable, Category = "HUD|Events")
	FOnHUDElementVisibilityChanged OnHUDElementVisibilityChanged;

	UPROPERTY(BlueprintAssignable, Category = "HUD|Events")
	FOnHealthChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "HUD|Events")
	FOnMaxHealthChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "HUD|Events")
	FOnExpChangedSignature OnExpChanged;

	UPROPERTY(BlueprintAssignable, Category = "HUD|Events")
	FOnMaxExpChangedSignature OnMaxExpChanged;

	UPROPERTY(BlueprintAssignable, Category = "HUD|Events")
	FOnLevelChangedSignature OnLevelChanged;

private:
	TMap<EHUDElement, bool> HUDElementVisibility;

};
