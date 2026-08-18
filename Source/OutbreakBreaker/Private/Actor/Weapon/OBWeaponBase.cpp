#include "Actor/Weapon/OBWeaponBase.h"
#include "Character/OBCharacter.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Attributes/OBWeaponAttributeSetBase.h"
#include "Component/OBWeaponComponent.h"

AOBWeaponBase::AOBWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;
}

void AOBWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

void AOBWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateWeaponMovement(DeltaTime);
}

void AOBWeaponBase::UpdateWeaponMovement_Implementation(float DeltaTime)
{
	if (TargetAnchor)
	{
		// 위치 추적
		FVector CurrentLocation = GetActorLocation();
		FVector TargetLocation = TargetAnchor->GetComponentLocation();
		FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, FollowSpeed);
		SetActorLocation(NewLocation);

		// 회전 동기화
		FRotator CurrentRotation = GetActorRotation();
		FRotator TargetRotation = TargetAnchor->GetComponentRotation();
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, FollowSpeed);
		SetActorRotation(NewRotation);
	}
}

void AOBWeaponBase::SetTargetAnchor(USceneComponent* InAnchor)
{
	TargetAnchor = InAnchor;
}

void AOBWeaponBase::ToggleWeaponMode(bool bIsActiveMode)
{
	if (!OwnerCharacter)
	{
		return;
	}

	FGameplayTag TargetTag = bIsActiveMode ? ActiveTag : PassiveTag;

	if (CurrentTag == TargetTag)
	{
		return;
	}

	OwnerCharacter->ModifyGameplayTag(CurrentTag, false);

	CurrentTag = TargetTag;

	OwnerCharacter->ModifyGameplayTag(CurrentTag, true);
}

void AOBWeaponBase::OnSpawnFromPool()
{
	WeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));
}

void AOBWeaponBase::OnRecycleToPool()
{
	WeaponMesh->SetCollisionProfileName(TEXT("PoolStorage"));

	if (!OwnerCharacter)
	{
		return;
	}

	OwnerCharacter->ModifyGameplayTag(ActiveTag, false);
	OwnerCharacter->ModifyGameplayTag(PassiveTag, false);

	OwnerCharacter->UnregisterWeaponFromMap(ActiveTag, this);
	OwnerCharacter->UnregisterWeaponFromMap(PassiveTag, this);

	WeaponComponent->DeinitializeWeapon(WeaponType);

	TargetAnchor = nullptr;
	OwnerCharacter = nullptr;
	WeaponComponent = nullptr;
	CurrentTag = FGameplayTag::EmptyTag;
}

void AOBWeaponBase::InitializeSpawnedObject(int32 InLevel, AActor* InTargetPlayer)
{
	if (!InTargetPlayer)
	{
		return;
	}

	OwnerCharacter = Cast<AOBCharacter>(InTargetPlayer);
	if (!OwnerCharacter)
	{
		return;
	}

	SetTargetAnchor(OwnerCharacter->GetWeaponAnchor(WeaponType));

	WeaponComponent = OwnerCharacter->FindComponentByClass<UOBWeaponComponent>();
	if (WeaponComponent)
	{
		CurrentTag = PassiveTag;

		OwnerCharacter->RegisterWeaponToMap(ActiveTag, this);
		OwnerCharacter->RegisterWeaponToMap(PassiveTag, this);

		OwnerCharacter->ModifyGameplayTag(CurrentTag, true);

		WeaponComponent->InitializeOrUpgradeWeapon(WeaponType, InLevel);
	}
}
