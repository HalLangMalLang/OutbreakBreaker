#include "Subsystems/JMUISubsystem.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AttributeSet.h"
#include "Core/JMUserWidgetBase.h" 

namespace
{
	FString ParamsTypeToString(EJMWidgetControllerParamsType Type)
	{
		return StaticEnum<EJMWidgetControllerParamsType>()->GetNameStringByValue(static_cast<int64>(Type));
	}
}

FJMWidgetControllerParams UJMUISubsystem::MakeWidgetControllerParams(APlayerController* PC)
{
	if (!PC)
	{
		UE_LOG(LogTemp, Error, TEXT("[JMUISubsystem] MakeWidgetControllerParams: PC is null"));
		return FJMWidgetControllerParams();
	}

	// ──────────────────────────────────────────────────────────
	// PlayerState
	// ──────────────────────────────────────────────────────────
	APlayerState* PS = PC->GetPlayerState<APlayerState>();
	if (!PS)
	{
		UE_LOG(LogTemp, Warning, TEXT("[JMUISubsystem] PlayerState not replicated yet"));
	}

	// ──────────────────────────────────────────────────────────
	// AbilitySystemComponent 탐색 (PlayerState → Pawn)
	// ──────────────────────────────────────────────────────────
	UAbilitySystemComponent* ASC = nullptr;

	// 1순위 : PlayerState
	if (PS)
	{
		if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(PS))
		{
			ASC = ASI->GetAbilitySystemComponent();
		}
	}

	// 2순위 : Pawn
	if (!ASC)
	{
		if (APawn* Pawn = PC->GetPawn())
		{
			if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(Pawn))
			{
				ASC = ASI->GetAbilitySystemComponent();
			}
		}
	}
	if (!ASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("[JMUISubsystem] ASC not found on PlayerState or Pawn"));
	}


	// ──────────────────────────────────────────────────────────
	// AttributeSet 탐색 (첫 번째 AttributeSet 사용)
	// ──────────────────────────────────────────────────────────
	const UAttributeSet* AS = nullptr;
	if (ASC)
	{
		const TArray<UAttributeSet*>& SpawnedAttributes = ASC->GetSpawnedAttributes();
		if (SpawnedAttributes.Num() > 0)
		{
			AS = SpawnedAttributes[0];
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[JMUISubsystem] ASC has no AttributeSets"));
		}
	}

	// ──────────────────────────────────────────────────────────
	// GameState
	// ──────────────────────────────────────────────────────────
	AGameStateBase* GS = PC->GetWorld() ? PC->GetWorld()->GetGameState() : nullptr;
	if (!GS)
	{
		UE_LOG(LogTemp, Warning, TEXT("[JMUISubsystem] GameState is null"));
	}

	return FJMWidgetControllerParams(PC, PS, ASC, GS);
}

FJMWidgetControllerParams UJMUISubsystem::MakeSimpleParams(APlayerController* PC)
{
	if (!PC)
	{
		UE_LOG(LogTemp, Error, TEXT("[JMUISubsystem] MakeSimpleParams: PC is null"));
		return FJMWidgetControllerParams();
	}

	APlayerState* PS = PC->GetPlayerState<APlayerState>();
	if (!PS)
	{
		UE_LOG(LogTemp, Warning, TEXT("[JMUISubsystem] PlayerState not replicated yet"));
	}

	AGameStateBase* GS = PC->GetWorld() ? PC->GetWorld()->GetGameState() : nullptr;

	// ASC/AS는 nullptr로 설정
	return FJMWidgetControllerParams(PC, PS, nullptr, GS);
}

UJMWidgetControllerBase* UJMUISubsystem::GetWidgetController(TSubclassOf<UJMWidgetControllerBase> ControllerClass, APlayerController* PC, EJMWidgetControllerParamsType ParamsType)
{
	if (!PC || !ControllerClass)
	{
		UE_LOG(LogTemp, Error, TEXT("[JMUISubsystem] GetWidgetController : Invalid parameters"));
		return nullptr;
	}

	// ──────────────────────────────────────────────────────────
	// 캐시 확인 (ControllerClass만으로 키 사용)
	// ──────────────────────────────────────────────────────────
	if (const FJMCachedController* Cached = WidgetControllers.Find(ControllerClass))
	{
		if (Cached->ParamsType != ParamsType)
		{
			UE_LOG(LogTemp, Error, TEXT("[JMUISubsystem] %s was cached as %s but requested as %s! Returning cached instance anyway - check your call sites."),
				*ControllerClass->GetName(),
				*ParamsTypeToString(Cached->ParamsType),
				*ParamsTypeToString(ParamsType));
		}

		UE_LOG(LogTemp, Log, TEXT("[JMUISubsystem] Returning cached controller : %s"), *ControllerClass->GetName());
		return Cached->Controller;
	}

	// ──────────────────────────────────────────────────────────
	// 파라미터 생성 (타입에 따라 분기)
	// ──────────────────────────────────────────────────────────
	FJMWidgetControllerParams Params;

	switch (ParamsType)
	{
	case EJMWidgetControllerParamsType::Simple:
		Params = MakeSimpleParams(PC);
		if (!Params.IsValidSimple())
		{
			UE_LOG(LogTemp, Warning, TEXT("[JMUISubsystem] Simple params not ready (PC/PS missing)"));
			return nullptr;
		}
		break;

	case EJMWidgetControllerParamsType::WithGAS:
		Params = MakeWidgetControllerParams(PC);
		if (!Params.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("[JMUISubsystem] GAS params not ready (ASC/AS/GS/PS missing)"));
			return nullptr;
		}
		break;
	}

	// ──────────────────────────────────────────────────────────
	// 컨트롤러 생성 (자식 클래스 타입으로 생성)
	// ──────────────────────────────────────────────────────────
	UJMWidgetControllerBase* NewController = NewObject<UJMWidgetControllerBase>(this, ControllerClass);

	NewController->SetWidgetControllerParams(Params);
	NewController->BindCallbacksToDependencies();

	// ──────────────────────────────────────────────────────────
	// 캐시에 저장
	// ──────────────────────────────────────────────────────────
	FJMCachedController Entry;
	Entry.Controller = NewController;
	Entry.ParamsType = ParamsType;
	WidgetControllers.Add(ControllerClass, Entry);

	UE_LOG(LogTemp, Log, TEXT("[JMUISubsystem] Created new controller: %s (ParamsType: %s)"),
		*ControllerClass->GetName(),
		*ParamsTypeToString(ParamsType));

	return NewController;
}

UJMUserWidgetBase* UJMUISubsystem::JMCreateWidget(TSubclassOf<UJMUserWidgetBase> WidgetClass)
{
	if (!WidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("[JMUISubsystem] JMCreateWidget: WidgetClass is null"));
		return nullptr;
	}

	APlayerController* PC = GetGameInstance()->GetFirstLocalPlayerController();
	if (!PC)
	{
		UE_LOG(LogTemp, Error, TEXT("[JMUISubsystem] JMCreateWidget: No local player controller"));
		return nullptr;
	}

	return CreateWidget<UJMUserWidgetBase>(PC, WidgetClass);
}