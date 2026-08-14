#include "UI/WidgetController/OBHUDWidgetController.h"
#include "AbilitySystem/Attributes/OBCharacterAttributeSet.h"

void UOBHUDWidgetController::BroadcastInitialValues()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	InitializeVisibility();

	if (const UOBCharacterAttributeSet* OBAS = Cast<UOBCharacterAttributeSet>(AbilitySystemComponent->GetAttributeSet(UOBCharacterAttributeSet::StaticClass())))
	{
		OnHealthChanged.Broadcast(OBAS->GetHP());
		OnMaxHealthChanged.Broadcast(OBAS->GetMaxHP());
		OnExpChanged.Broadcast(OBAS->GetXP());
		OnMaxExpChanged.Broadcast(OBAS->GetMaxXP());
		OnLevelChanged.Broadcast(OBAS->GetLevel());
	}

	ShowAllHUD();
	//HideHUDElement(EHUDElement::BossHealthBar);
}

void UOBHUDWidgetController::BindCallbacksToDependencies()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UOBCharacterAttributeSet::GetHPAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data) {
		OnHealthChanged.Broadcast(Data.NewValue); });

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UOBCharacterAttributeSet::GetMaxHPAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data) {
		OnMaxHealthChanged.Broadcast(Data.NewValue); });

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UOBCharacterAttributeSet::GetXPAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data) {
		OnExpChanged.Broadcast(Data.NewValue); });

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UOBCharacterAttributeSet::GetMaxXPAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data) {
		OnMaxExpChanged.Broadcast(Data.NewValue); });

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UOBCharacterAttributeSet::GetLevelAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data) {
		OnLevelChanged.Broadcast(Data.NewValue); });
}

void UOBHUDWidgetController::InitializeVisibility()
{
	HUDElementVisibility.Add(EHUDElement::HealthBar, true);
	HUDElementVisibility.Add(EHUDElement::ExpBar, true);
	//HUDElementVisibility.Add(EHUDElement::WeaponSlots, true);
	//HUDElementVisibility.Add(EHUDElement::StageTimer, true);
	//HUDElementVisibility.Add(EHUDElement::BossHealthBar, true);
}

void UOBHUDWidgetController::ShowAllHUD()
{
	for (auto& Pair : HUDElementVisibility)
	{
		ShowHUDElement(Pair.Key);
	}
}

void UOBHUDWidgetController::HideAllHUD()
{
	for (auto& Pair : HUDElementVisibility)
	{
		HideHUDElement(Pair.Key);
	}
}

void UOBHUDWidgetController::ToggleAllHUD()
{
	for (auto& Pair : HUDElementVisibility)
	{
		ToggleHUDElement(Pair.Key);
	}
}

void UOBHUDWidgetController::ShowHUDElement(EHUDElement Element)
{
	if (bool* VisibilityPtr = HUDElementVisibility.Find(Element))
	{
		*VisibilityPtr = true;
		if (OnHUDElementVisibilityChanged.IsBound())
		{
			OnHUDElementVisibilityChanged.Broadcast(Element, true);
		}
	}
}

void UOBHUDWidgetController::HideHUDElement(EHUDElement Element)
{
	if (bool* VisibilityPtr = HUDElementVisibility.Find(Element))
	{
		*VisibilityPtr = false;
		if (OnHUDElementVisibilityChanged.IsBound())
		{
			OnHUDElementVisibilityChanged.Broadcast(Element, false);
		}
	}
}

void UOBHUDWidgetController::ToggleHUDElement(EHUDElement Element)
{
	if (bool* VisibilityPtr = HUDElementVisibility.Find(Element))
	{
		*VisibilityPtr = !(*VisibilityPtr);
		if (OnHUDElementVisibilityChanged.IsBound())
		{
			OnHUDElementVisibilityChanged.Broadcast(Element, *VisibilityPtr);
		}
	}
}

bool UOBHUDWidgetController::IsHUDElementVisible(EHUDElement Element) const
{
	if (const bool* VisibilityPtr = HUDElementVisibility.Find(Element))
	{
		return *VisibilityPtr;
	}

	return false;
}
