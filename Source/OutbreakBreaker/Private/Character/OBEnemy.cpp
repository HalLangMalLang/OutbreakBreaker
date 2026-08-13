#include "Character/OBEnemy.h"
#include "AbilitySystem/OBAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/OBEnemyAttributeSet.h"
#include "Actor/OBEffectActor.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

AOBEnemy::AOBEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UOBAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UOBEnemyAttributeSet>(TEXT("AttributeSet"));
}

void AOBEnemy::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityActorInfo();

	TargetPlayer = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

void AOBEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsDead || !TargetPlayer)
	{
		return;
	}

	FVector CurrentLocation = GetActorLocation();
	FVector TargetLocation = TargetPlayer->GetActorLocation();
	TargetLocation.Z = CurrentLocation.Z;

	float DistanceToPlayer = FVector::Dist(CurrentLocation, TargetLocation);

	FVector LookAtDirection = (TargetLocation - CurrentLocation).GetSafeNormal();
	FRotator TargetRotation = LookAtDirection.Rotation();
	FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 10.0f);
	SetActorRotation(NewRotation);

	bool bIsCurrentlyAttacking = AbilitySystemComponent && AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.Combat.Attacking")));

	if (DistanceToPlayer > AttackRange && !bIsCurrentlyAttacking)
	{
		AddMovementInput(LookAtDirection, 1.0f);
	}
	else
	{
		if (GetCharacterMovement())
		{
			GetCharacterMovement()->StopMovementImmediately();
		}
	
		if (!bIsCurrentlyAttacking)
		{

			AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(AttackAbilityTag));
		}
	}
}

void AOBEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	InitializeDefaultAttributes();
	InitializeAttributeDelegates();

	AddCharacterAbilities();
}

void AOBEnemy::InitializeAttributeDelegates()
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		if (const UOBEnemyAttributeSet* OBAS = Cast<UOBEnemyAttributeSet>(AttributeSet))
		{
			const_cast<UOBEnemyAttributeSet*>(OBAS)->OnEnemyDeadDelegate.AddUObject(this, &AOBEnemy::OnCharacterDeathProcessed);
			const_cast<UOBEnemyAttributeSet*>(OBAS)->OnSpeedEnemyDelegate.AddUObject(this, &AOBEnemy::OnMoveSpeedVelocityUpdated);

			OBAS->OnSpeedEnemyDelegate.Broadcast(OBAS->GetMoveSpeed());
		}
	}
}

void AOBEnemy::OnCharacterDeathProcessed(AActor* Destroyer)
{
	Super::OnCharacterDeathProcessed(Destroyer);

	bIsDead = true;

	if (GetController())
	{
		GetController()->StopMovement();
	}

	UWorld* World = GetWorld();
	if (World && XPGemClass)
	{
		FVector DeathLocation = GetActorLocation();
		DeathLocation.Z += 20.0f;

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(DeathLocation);
		SpawnTransform.SetRotation(FQuat::Identity);
		SpawnTransform.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Owner = nullptr;

		if (AOBEffectActor* SpawnedGem = World->SpawnActor<AOBEffectActor>(XPGemClass, SpawnTransform, SpawnParams))
		{
			if (const UOBEnemyAttributeSet* EnemySet = Cast<UOBEnemyAttributeSet>(AttributeSet))
			{
				SpawnedGem->DamageCauser = Destroyer;
				SpawnedGem->SetActorLevel(EnemySet->GetLevel());
			}
		}
	}
}

