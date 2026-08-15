#include "Core/ObjectPoolSubsystem.h"
#include "Core/PooledObject.h"
#include "Engine/World.h"
#include "Core/PoolableInterface.h"


void UObjectPoolSubsystem::SetupPools(const TMap<FGameplayTag, FPooledObjectData>& InPooledObjectData)
{
	if (Pools.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ObjectPoolSubsystem] SetupPools called while pools already exist. Clearing old pools first."));
		ClearPools();
	}

	PooledObjectData = InPooledObjectData;
	FActorSpawnParameters SpawnParams;

	for (const TPair<FGameplayTag, FPooledObjectData>& ObjectData : PooledObjectData)
	{
		FGameplayTag CurrentTag = ObjectData.Key;
		const FPooledObjectData& Data = ObjectData.Value;

		if (!Data.ActorTemplate)
		{
			continue;
		}

		if (!Data.ActorTemplate->ImplementsInterface(UPoolableInterface::StaticClass()))
		{
			UE_LOG(LogTemp, Error, TEXT("[ObjectPoolSubsystem] %s 클래스는 UPoolableInterface를 구현하지 않아 풀에 등록할 수 없습니다."), *Data.ActorTemplate->GetName());
			continue;
		}

		FSingleObjectPool CurrentPoolIndex;

		SpawnParams.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		for (int32 ObjectIndex = 0; ObjectIndex < Data.PoolSize; ObjectIndex++)
		{
			FString UniqueNameStr = FString::Printf(TEXT("%s_%d"), *CurrentTag.ToString(), ObjectIndex);
			SpawnParams.Name = FName(*UniqueNameStr);

			AActor* SpawnedActor = GetWorld()->SpawnActor(Data.ActorTemplate, &FVector::ZeroVector, &FRotator::ZeroRotator, SpawnParams);

			if (!IsValid(SpawnedActor))
			{
				continue;
			}

#if WITH_EDITOR 
			SpawnedActor->SetActorLabel(SpawnedActor->GetName());
#endif

			UPooledObject* PoolComp = NewObject<UPooledObject>(SpawnedActor);
			PoolComp->RegisterComponent();

			SpawnedActor->AddInstanceComponent(PoolComp);

			PoolComp->Init(this);

			CurrentPoolIndex.PooledObjects.Add(PoolComp);

			SpawnedActor->SetActorHiddenInGame(true);
			SpawnedActor->SetActorEnableCollision(false);
			SpawnedActor->SetActorTickEnabled(false);
			if (IPoolableInterface* Poolable = Cast<IPoolableInterface>(SpawnedActor))
			{
				Poolable->OnRecycleToPool();
			}

			if (PoolAnchor)
			{
				SpawnedActor->AttachToActor(PoolAnchor, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			}
		}
		Pools.Add(CurrentTag, CurrentPoolIndex);
	}
}

void UObjectPoolSubsystem::ClearPools()
{
	BroadcastPoolerCleanup();

	for (auto& PoolPair : Pools)
	{
		FSingleObjectPool& CurrentPool = PoolPair.Value;

		for (UPooledObject* PoolComp : CurrentPool.PooledObjects)
		{
			if (IsValid(PoolComp))
			{
				AActor* OwnerActor = PoolComp->GetOwner();
				if (IsValid(OwnerActor))
				{
					OwnerActor->Destroy();
				}
			}
		}
		CurrentPool.PooledObjects.Reset();
	}

	PooledObjectData.Reset();
	Pools.Reset();
}

AActor* UObjectPoolSubsystem::GetPooledActor(const FGameplayTag& PoolName)
{
	if (!Pools.Contains(PoolName))
	{
		return nullptr;
	}

	FSingleObjectPool& TargetPool = Pools[PoolName];
	int32 PooledObjectCount = TargetPool.PooledObjects.Num();
	int32 FirstAvailable = -1;

	for (int32 i = 0; i < PooledObjectCount; i++)
	{
		if (TargetPool.PooledObjects[i] != nullptr)
		{
			if (!TargetPool.PooledObjects[i]->bIsPoolActive)
			{
				FirstAvailable = i;
				break;
			}
		}
		else
		{
			RegenItem(PoolName, i);

			if (TargetPool.PooledObjects[i] != nullptr)
			{
				FirstAvailable = i;
				break;
			}
		}
	}

	if (FirstAvailable >= 0)
	{
		UPooledObject* ToReturn = TargetPool.PooledObjects[FirstAvailable];
		ToReturn->bIsPoolActive = true;

		OnPoolerCleanup.AddUniqueDynamic(ToReturn, &UPooledObject::RecycleSelf);

		AActor* ToReturnActor = ToReturn->GetOwner();
		if (IsValid(ToReturnActor))
		{
			ToReturnActor->SetActorHiddenInGame(false);
			ToReturnActor->SetActorEnableCollision(true);
			ToReturnActor->SetActorTickEnabled(true);
			if (IPoolableInterface* Poolable = Cast<IPoolableInterface>(ToReturnActor))
			{
				Poolable->OnSpawnFromPool();
			}

			ToReturnActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			return ToReturnActor;
		}
	}

	if (!PooledObjectData[PoolName].bCanGrow)
	{
		return nullptr;
	}

	FActorSpawnParameters SpawnParams;

	FString UniqueNameStr = FString::Printf(TEXT("%s_Grow_%d"), *PoolName.ToString(), PooledObjectCount);
	SpawnParams.Name = FName(*UniqueNameStr);

	SpawnParams.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* SpawnedActor = GetWorld()->SpawnActor(PooledObjectData[PoolName].ActorTemplate, &FVector::ZeroVector, &FRotator::ZeroRotator, SpawnParams);
	if (!IsValid(SpawnedActor))
	{
		return nullptr;
	}

#if WITH_EDITOR
	SpawnedActor->SetActorLabel(SpawnedActor->GetName());
#endif

	UPooledObject* PoolComp = NewObject<UPooledObject>(SpawnedActor);
	if (PoolComp)
	{
		PoolComp->RegisterComponent();
		SpawnedActor->AddInstanceComponent(PoolComp);
		PoolComp->Init(this);

		TargetPool.PooledObjects.Add(PoolComp);

		PoolComp->bIsPoolActive = true;
		OnPoolerCleanup.AddUniqueDynamic(PoolComp, &UPooledObject::RecycleSelf);

		if (IPoolableInterface* Poolable = Cast<IPoolableInterface>(SpawnedActor))
		{
			Poolable->OnSpawnFromPool();
		}
	}

	return SpawnedActor;
}


void UObjectPoolSubsystem::RecycleActor(AActor* PooledActor)
{
	if (!IsValid(PooledActor))
	{
		return;
	}

	if (UPooledObject* PoolCompRef = Cast<UPooledObject>(PooledActor->GetComponentByClass(UPooledObject::StaticClass())))
	{
		RecyclePooledObject(PoolCompRef);
	}
}

void UObjectPoolSubsystem::RecyclePooledObject(UPooledObject* PoolCompRef)
{
	if (!IsValid(PoolCompRef))
	{
		return;
	}

	OnPoolerCleanup.RemoveDynamic(PoolCompRef, &UPooledObject::RecycleSelf);

	PoolCompRef->bIsPoolActive = false;

	AActor* ReturningActor = PoolCompRef->GetOwner();
	if (IsValid(ReturningActor))
	{
		ReturningActor->SetActorEnableCollision(false);
		ReturningActor->SetActorHiddenInGame(true);
		ReturningActor->SetActorTickEnabled(false);

		if (IPoolableInterface* Poolable = Cast<IPoolableInterface>(ReturningActor))
		{
			Poolable->OnRecycleToPool();
		}

		if (PoolAnchor)
		{
			ReturningActor->AttachToActor(PoolAnchor, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
	}
}

void UObjectPoolSubsystem::BroadcastPoolerCleanup()
{
	OnPoolerCleanup.Broadcast();
}

void UObjectPoolSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UE_LOG(LogTemp, Log, TEXT("ObjectPoolSubsystem Initialized"));

	PostWorldInitHandle = FWorldDelegates::OnPostWorldInitialization.AddUObject(this, &UObjectPoolSubsystem::OnWorldInitialized);
	WorldCleanupHandle = FWorldDelegates::OnWorldCleanup.AddUObject(this, &UObjectPoolSubsystem::OnWorldCleanup);
}

void UObjectPoolSubsystem::Deinitialize()
{
	UE_LOG(LogTemp, Log, TEXT("ObjectPoolSubsystem Deinitialized"));

	if (PostWorldInitHandle.IsValid())
	{
		FWorldDelegates::OnPostWorldInitialization.Remove(PostWorldInitHandle);
		PostWorldInitHandle.Reset();
	}

	if (WorldCleanupHandle.IsValid())
	{
		FWorldDelegates::OnWorldCleanup.Remove(WorldCleanupHandle);
		WorldCleanupHandle.Reset();
	}

	Super::Deinitialize();
}

void UObjectPoolSubsystem::RegenItem(const FGameplayTag& PoolName, int32 PositionIndex)
{
	if (!PooledObjectData.Contains(PoolName) || !Pools.Contains(PoolName))
	{
		return;
	}

	if (!Pools[PoolName].PooledObjects.IsValidIndex(PositionIndex))
	{
		return;
	}

	FActorSpawnParameters SpawnParams;

	FString UniqueNameStr = FString::Printf(TEXT("%s_Regen_%d"), *PoolName.ToString(), PositionIndex);
	SpawnParams.Name = FName(*UniqueNameStr);

	SpawnParams.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* SpawnedActor = GetWorld()->SpawnActor(PooledObjectData[PoolName].ActorTemplate, &FVector::ZeroVector, &FRotator::ZeroRotator, SpawnParams);
	if (!IsValid(SpawnedActor))
	{
		return;
	}

#if WITH_EDITOR
	SpawnedActor->SetActorLabel(SpawnedActor->GetName());
#endif

	UPooledObject* PoolComp = NewObject<UPooledObject>(SpawnedActor);
	if (PoolComp)
	{
		PoolComp->RegisterComponent();
		SpawnedActor->AddInstanceComponent(PoolComp);
		PoolComp->Init(this);

		Pools[PoolName].PooledObjects[PositionIndex] = PoolComp;

		SpawnedActor->SetActorHiddenInGame(true);
		SpawnedActor->SetActorEnableCollision(false);
		SpawnedActor->SetActorTickEnabled(false);

		if (IPoolableInterface* Poolable = Cast<IPoolableInterface>(SpawnedActor))
		{
			Poolable->OnRecycleToPool();
		}

		if (PoolAnchor)
		{
			SpawnedActor->AttachToActor(PoolAnchor, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
	}
}

void UObjectPoolSubsystem::OnWorldInitialized(UWorld* World, const UWorld::InitializationValues IValues)
{
	// 게임 월드인지 확인
	if (!World || !World->IsGameWorld())
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	PoolAnchor = World->SpawnActor(
		AActor::StaticClass(),
		&FVector::ZeroVector,
		&FRotator::ZeroRotator,
		SpawnParams
	);

	if (IsValid(PoolAnchor))
	{
		// 루트 컴포넌트 생성
		if (USceneComponent* RootComp = NewObject<USceneComponent>(PoolAnchor, TEXT("Root")))
		{
			RootComp->RegisterComponent();
			PoolAnchor->SetRootComponent(RootComp);
		}

#if WITH_EDITOR 
		PoolAnchor->SetActorLabel(TEXT("ObjectPoolAnchor"));
#endif
	}
}
void UObjectPoolSubsystem::OnWorldCleanup(UWorld* World, bool bSessionEnded, bool bCleanupResources)
{
	if (!World || !World->IsGameWorld())
	{
		return;
	}

	// 풀 정리
	ClearPools();

	if (IsValid(PoolAnchor))
	{
		PoolAnchor->Destroy();
	}

	PoolAnchor = nullptr;
}