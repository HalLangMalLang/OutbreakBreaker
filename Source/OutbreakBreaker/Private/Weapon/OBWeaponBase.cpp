#include "Weapon/OBWeaponBase.h"
#include "Character/OBCharacter.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Attributes/OBWeaponAttributeSetBase.h"


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

	if (TargetAnchor)
	{
		// 위치 추적(FMath::VInterpTo 보간 이동)
		FVector CurrentLocation = GetActorLocation();
		FVector TargetLocation = TargetAnchor->GetComponentLocation();
		FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, FollowSpeed);
		SetActorLocation(NewLocation);

		// 회전 동기화(FMath::RInterpTo 부드러운 추적 회전)
		FRotator CurrentRotation = GetActorRotation();
		FRotator TargetRotation = TargetAnchor->GetComponentRotation();
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, FollowSpeed);
		SetActorRotation(NewRotation);
	}
}

void AOBWeaponBase::SetOwner(AActor* NewOwner)
{
	Super::SetOwner(NewOwner);

	OwnerCharacter = Cast<AOBCharacter>(NewOwner);

	if (OwnerCharacter)
	{
		CurrentTag = PassiveTag;
		OwnerCharacter->RegisterWeaponToMap(ActiveTag, this);
		OwnerCharacter->RegisterWeaponToMap(PassiveTag, this);

		OwnerCharacter->SetWeaponTag(CurrentTag, true);
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

	OwnerCharacter->SetWeaponTag(CurrentTag, false);

	CurrentTag = TargetTag;

	OwnerCharacter->SetWeaponTag(CurrentTag, true);
}