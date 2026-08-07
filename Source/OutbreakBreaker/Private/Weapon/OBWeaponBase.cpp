#include "Weapon/OBWeaponBase.h"
#include "Character/OBCharacter.h"


AOBWeaponBase::AOBWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	CurrentWeaponState = EOBWeaponState::Passive;
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
		// 위치 추적 (FMath::VInterpTo 보간 이동)
		FVector CurrentLocation = GetActorLocation();
		FVector TargetLocation = TargetAnchor->GetComponentLocation();
		FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, FollowSpeed);
		SetActorLocation(NewLocation);

		// 회전 동기화 (FMath::RInterpTo 부드러운 추적 회전)
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
}

void AOBWeaponBase::SetTargetAnchor(USceneComponent* InAnchor)
{
	TargetAnchor = InAnchor;
}

void AOBWeaponBase::SetWeaponState(EOBWeaponState NewState)
{
	if (CurrentWeaponState == NewState)
	{
		return;
	}

	CurrentWeaponState = NewState;
}