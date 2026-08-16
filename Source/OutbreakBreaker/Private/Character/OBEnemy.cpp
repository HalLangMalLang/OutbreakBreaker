#include "Character/OBEnemy.h"
#include "AbilitySystem/OBAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/OBEnemyAttributeSet.h"
#include "Actor/OBPooledEffectActor.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Core/ObjectPoolSubsystem.h"

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
	if (World && SpawnGemTag.IsValid())
	{
		FVector DeathLocation = GetActorLocation();
		DeathLocation.Z += 20.0f;

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(DeathLocation);
		SpawnTransform.SetRotation(FQuat::Identity);
		SpawnTransform.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

		if (UOBEnemyAttributeSet* AS = Cast<UOBEnemyAttributeSet>(AttributeSet))
		{
			if (UObjectPoolSubsystem* PoolSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UObjectPoolSubsystem>())
			{
				if (AActor* EnemyActor = PoolSubsystem->GetPooledActor(SpawnGemTag))
				{
					//EnemyActor->SetActorTransform(SpawnTransform, false, nullptr, ETeleportType::TeleportPhysics);
					EnemyActor->SetActorLocation(DeathLocation, false, nullptr, ETeleportType::TeleportPhysics);

					if (IOBSpawnableInterface* Spawnable = Cast<IOBSpawnableInterface>(EnemyActor))
					{
						Spawnable->InitializeSpawnedObject(AS->GetBountyXP(), TargetPlayer);
					}
				}
			}
		}

	}
}

void AOBEnemy::OnSpawnFromPool()
{
	if (UCapsuleComponent* Capsule = GetCapsuleComponent())
	{
		Capsule->SetCollisionProfileName(TEXT("Pawn"));
	}

	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->SetComponentTickEnabled(true);
		MoveComp->SetMovementMode(MOVE_Walking);
	}
}

void AOBEnemy::OnRecycleToPool()
{
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->StopMovementImmediately();
		MoveComp->DisableMovement();
		MoveComp->SetComponentTickEnabled(false);
	}

	if (UCapsuleComponent* Capsule = GetCapsuleComponent())
	{
		Capsule->SetCollisionProfileName(TEXT("PoolStorage"));
	}

	if (GetMesh())
	{
		GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	}
}

void AOBEnemy::InitializeSpawnedObject(int32 InLevel, AActor* InTargetPlayer)
{
	TargetPlayer = InTargetPlayer;

	if (!AbilitySystemComponent)
	{
		return;
	}

	if (LevelInitEffect)
	{
		FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
		ContextHandle.AddInstigator(this, this);

		FGameplayEffectSpecHandle LevelSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(LevelInitEffect, 1.f, ContextHandle);
		if (LevelSpecHandle.IsValid())
		{
			FGameplayTag LevelDataTag = FGameplayTag::RequestGameplayTag(FName("Data.MonsterLevel"));
			LevelSpecHandle.Data.Get()->SetSetByCallerMagnitude(LevelDataTag, static_cast<float>(InLevel));

			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*LevelSpecHandle.Data.Get());
		}
	}

	InitializeDefaultAttributes();
}
