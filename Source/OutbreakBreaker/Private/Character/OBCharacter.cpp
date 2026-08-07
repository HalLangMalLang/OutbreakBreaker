#include "Character/OBCharacter.h"
#include "AbilitySystem/OBAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Player/OBPlayerState.h"
#include "Input/OBInputConfig.h"
#include "Input/OBEnhancedInputComponent.h"
#include "Data/OBWeaponInfoRow.h"

AOBCharacter::AOBCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// 캡슐 컴포넌트 설정 및 안전성 검사
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp != nullptr);
	CapsuleComp->InitCapsuleSize(42.f, 96.0f);

	// 컨트롤러 회전이 캐릭터에 영향을 주지 않도록 설정
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 무브먼트 컴포넌트 설정 및 안전성 검사
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	check(MoveComp != nullptr);
	MoveComp->bOrientRotationToMovement = false;
	MoveComp->bUseControllerDesiredRotation = false;
	MoveComp->MaxWalkSpeed = 500.f;

	// 카메라 암(SpringArm) 설정
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	check(CameraArm != nullptr);
	CameraArm->SetupAttachment(RootComponent);
	CameraArm->TargetArmLength = 1000.0f;
	CameraArm->bUsePawnControlRotation = false;

	// 캐릭터가 회전해도 카메라는 월드 기준 고정되도록 상속 해제
	CameraArm->bInheritPitch = false;
	CameraArm->bInheritRoll = false;
	CameraArm->bInheritYaw = false;
	CameraArm->bDoCollisionTest = false; // 벽에 가려도 카메라가 당겨지지 않음 (쿼터뷰 필수)

	// 생성자 단계에서는 Relative로 각도를 꺾어주는 것이 엔진 내부적으로 가장 안전합니다.
	// (어차피 Inherit을 다 껐기 때문에 Relative가 곧 월드 기준 고정 각도가 됩니다)
	CameraArm->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));

	// 카메라 컴포넌트 설정
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	check(Camera != nullptr);
	Camera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
}

void AOBCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocallyControlled())
	{
		return;
	}

	SetupMouseInterface();
}

void AOBCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsLocallyControlled())
	{
		return;
	}

	RotateToCursor(DeltaTime);

	UpdateDodgePosition(DeltaTime);
}

void AOBCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AOBCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UOBEnhancedInputComponent* OBEnhancedInputComponent = CastChecked<UOBEnhancedInputComponent>(PlayerInputComponent);
	if (!OBEnhancedInputComponent)
	{
		return;
	}

	OBEnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AOBCharacter::Move);

	OBEnhancedInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AOBCharacter::Move(const FInputActionValue& Value)
{
	if (AbilitySystemComponent && AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Player.Ability.LockMovement"))))
	{
		return;
	}

	if (Controller)
	{
		FVector2D MovementVector = Value.Get<FVector2D>();

		// 컨트롤러 회전에 의존하지 않고 월드 축 고정으로 명시
		const FVector ForwardDirection = FVector::ForwardVector;
		const FVector RightDirection = FVector::RightVector;

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AOBCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();

	AddCharacterAbilities();
}

void AOBCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilityActorInfo();
}

void AOBCharacter::RotateToCursor(float DeltaTime)
{
	if (AbilitySystemComponent && AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Player.Ability.LockRotation"))))
	{
		return;
	}

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		FHitResult HitResult;
		// 블루프린트의 'Get Hit Result Under Cursor by Channel' (Visibility 채널)
		if (PC->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, HitResult))
		{
			// 마우스가 찍힌 월드 좌표 (Location 핀 분할)
			FVector TargetLocation = HitResult.ImpactPoint;
			FVector CharacterLocation = GetActorLocation();

			// 캐릭터가 땅이나 하늘을 보며 기울어지는 버그 방지 (Pitch/Roll 고정 효과)
			TargetLocation.Z = CharacterLocation.Z;

			// 블루프린트의 'Find Look at Rotation'
			FRotator TargetRotation = FRotationMatrix::MakeFromX(TargetLocation - CharacterLocation).Rotator();

			// 블루프린트의 'RInterp To' (회전 보간)
			FRotator CurrentRotation = GetActorRotation();

			FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationInterpSpeed);

			// 블루프린트의 'Set Actor Rotation'
			SetActorRotation(NewRotation);
		}
	}
}

void AOBCharacter::SetupMouseInterface()
{
	APlayerController* PC = Cast<APlayerController>(GetController());

	checkf(PC != nullptr, TEXT("[%s] BeginPlay 단계에서 PlayerController를 찾을 수 없습니다!"), *GetName());

	PC->bShowMouseCursor = true;

	// 탑다운 게임용 입력 모드 설정 (마우스가 게임 화면을 벗어나지 않고, 클릭해도 화면이 안 돌아가게 방지)
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // 창 모드일 때 마우스 가둘지 여부 (자유롭게 이동 가능)
	InputModeData.SetHideCursorDuringCapture(false);                         // 클릭 중에도 마우스 유지

	PC->SetInputMode(InputModeData);
}

UAbilitySystemComponent* AOBCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AOBCharacter::InitAbilityActorInfo()
{
	AOBPlayerState* OBPS = GetPlayerState<AOBPlayerState>();
	check(OBPS);
	OBPS->GetAbilitySystemComponent()->InitAbilityActorInfo(OBPS, this);

	AbilitySystemComponent = OBPS->GetAbilitySystemComponent();
	CharacterAttributeSet = OBPS->GetAttributeSet();

	PlasmaRifleAttributes = OBPS->GetPlasmaRifleAttributeSet();
	GravityHammerAttributes = OBPS->GetGravityHammerAttributeSet();
	SpreadShotgunAttributes = OBPS->GetSpreadShotgunAttributeSet();

	DefenseDroneAttributes = OBPS->GetDefenseDroneAttributeSet();
	MagnetMineAttributes = OBPS->GetMagnetMineAttributeSet();
	AuraModuleAttributes = OBPS->GetAuraModuleAttributeSet();

	InitializeDefaultAttributes();
}

void AOBCharacter::AddCharacterAbilities()
{
	if (UOBAbilitySystemComponent* OBAbilitySystemComponent = Cast<UOBAbilitySystemComponent>(AbilitySystemComponent))
	{
		OBAbilitySystemComponent->AddCharacterAbilities(StartupAbilities);
	}
}

void AOBCharacter::DodgingLaunch()
{
	DodgeStartLocation = GetActorLocation();
	DodgeDirection = GetDodgeDirection();
	DodgeElapsedTime = 0.f;
	bIsDodging = true;
}

FVector AOBCharacter::GetDodgeDirection() const
{
	FVector Velocity = GetCharacterMovement()->Velocity;
	Velocity.Z = 0.f;

	if (!Velocity.IsNearlyZero())
	{
		return Velocity.GetSafeNormal(); // 이동 중이면 실제 이동 방향
	}

	return GetActorForwardVector(); // 정지 상태면 조준 방향
}

void AOBCharacter::StopDodgingMovement()
{
	bIsDodging = false;
	DodgeElapsedTime = 0.f;
}

void AOBCharacter::UpdateDodgePosition(float DeltaTime)
{
	if (bIsDodging)
	{
		DodgeElapsedTime += DeltaTime;
		float Alpha = FMath::Clamp(DodgeElapsedTime / DodgeDuration, 0.f, 1.f);
		SetActorLocation(DodgeStartLocation + DodgeDirection * DodgeDistance * Alpha, true);

		if (Alpha >= 1.f)
		{
			StopDodgingMovement();
		}
	}
}

void AOBCharacter::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);

	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void AOBCharacter::ApplyWeaponEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, const FOBWeaponInfoRow& WeaponInfo)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	check(IsValid(ASC));
	check(GameplayEffectClass);

	// 컨텍스트 및 원본 스펙 핸들 생성
	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GameplayEffectClass, 1.f, ContextHandle);

	if (SpecHandle.IsValid() && SpecHandle.Data.IsValid())
	{
		FGameplayEffectSpec* Spec = SpecHandle.Data.Get();

		// 플라즈마 라이플
		Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Data.Weapon.PlasmaRifle.Damage")), WeaponInfo.PlasmaRifleDamage);
		Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Data.Weapon.PlasmaRifle.TickInterval")), WeaponInfo.TickInterval);
		Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Data.Weapon.PlasmaRifle.PassiveRange")), WeaponInfo.PassiveRange);
		Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Data.Weapon.PlasmaRifle.PenetrationCount")), WeaponInfo.PenetrationCount);

		// 그라비티 해머
		Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Data.Weapon.GravityHammer.Damage")), WeaponInfo.GravityHammerDamage);
		Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Data.Weapon.GravityHammer.SlamCooldown")), WeaponInfo.SlamCooldown);
		Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Data.Weapon.GravityHammer.KnockbackForce")), WeaponInfo.KnockbackForce);
		Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Data.Weapon.GravityHammer.PullRadius")), WeaponInfo.PullRadius);

		// 스프레드 샷건
		Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Data.Weapon.SpreadShotgun.DamagePerPellet")), WeaponInfo.DamagePerPellet);
		Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Data.Weapon.SpreadShotgun.PelletCount")), WeaponInfo.PelletCount);
		Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Data.Weapon.SpreadShotgun.SpreadAngle")), WeaponInfo.SpreadAngle);
		Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Data.Weapon.SpreadShotgun.FireCooldown")), WeaponInfo.FireCooldown);

		// 디펜스 드론
		Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Data.Weapon.DefenseDrone.PushForce")), WeaponInfo.PushForce);
		Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Data.Weapon.DefenseDrone.OrbitRadius")), WeaponInfo.OrbitRadius);
		Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Data.Weapon.DefenseDrone.DeflectCooldown")), WeaponInfo.DeflectCooldown);

		// 마그넷 마인
		Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Data.Weapon.MagnetMine.ExplosionDamage")), WeaponInfo.ExplosionDamage);
		Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Data.Weapon.MagnetMine.ExplosionRadius")), WeaponInfo.ExplosionRadius);
		Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Data.Weapon.MagnetMine.SlowAmount")), WeaponInfo.SlowAmount);
		Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Data.Weapon.MagnetMine.DropInterval")), WeaponInfo.DropInterval);

		// 아우라 모듈
		Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Data.Weapon.AuraModule.TickDamage")), WeaponInfo.TickDamage);
		Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Data.Weapon.AuraModule.AuraRadius")), WeaponInfo.AuraRadius);
		Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Data.Weapon.AuraModule.TickInterval")), WeaponInfo.AuraTickInterval);

		// 3. 최종 발송!
		ASC->ApplyGameplayEffectSpecToTarget(*Spec, ASC);
	}
}

void AOBCharacter::InitializeDefaultAttributes()
{
	ApplyEffectToSelf(InitAttributes, 1.f);
	ApplyEffectToSelf(FillAttributes, 1.f);

	if (WeaponMasterDataTable && WeaponAttributesInitialize)
	{
		FOBWeaponInfoRow* MasterRow = WeaponMasterDataTable->FindRow<FOBWeaponInfoRow>(TEXT("AllWeapon"), "");
		if (MasterRow)
		{
			ApplyWeaponEffectToSelf(WeaponAttributesInitialize, *MasterRow);
		}
	}
}

void AOBCharacter::AbilityInputTagPressed(FGameplayTag InputTag)
{

}

void AOBCharacter::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (UOBAbilitySystemComponent* OBAbilitySystemComponent = Cast<UOBAbilitySystemComponent>(AbilitySystemComponent))
	{
		OBAbilitySystemComponent->AbilityInputTagReleased(InputTag);
	}
}

void AOBCharacter::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (UOBAbilitySystemComponent* OBAbilitySystemComponent = Cast<UOBAbilitySystemComponent>(AbilitySystemComponent))
	{
		OBAbilitySystemComponent->AbilityInputTagHeld(InputTag);
	}
}