#include "Core/ObjectPoolSubsystem.h"
#include "Core/PooledObject.h"
#include "Engine/World.h"


void UObjectPoolSubsystem::SetupPools(const TMap<FName, FPooledObjectData>& InPooledObjectData)
{
	if (Pools.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ObjectPoolSubsystem] SetupPools called while pools already exist. Clearing old pools first."));
		ClearPools();
	}

	PooledObjectData = InPooledObjectData;
	FActorSpawnParameters SpawnParams;

	for (TPair<FName, FPooledObjectData>& ObjectData : PooledObjectData)
	{
		FSingleObjectPool CurrentPoolIndex;
		SpawnParams.Name = ObjectData.Key;

		SpawnParams.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		for (int32 ObjectIndex = 0; ObjectIndex < ObjectData.Value.PoolSize; ObjectIndex++)
		{
			AActor* SpawnedActor = GetWorld()->SpawnActor(ObjectData.Value.ActorTemplate, &FVector::ZeroVector, &FRotator::ZeroRotator, SpawnParams);

			SpawnedActor->SetActorLabel(SpawnedActor->GetName());

			UPooledObject* PoolComp = NewObject<UPooledObject>(SpawnedActor);
			PoolComp->RegisterComponent();

			SpawnedActor->AddInstanceComponent(PoolComp);

			PoolComp->Init(this);

			CurrentPoolIndex.PooledObjects.Add(PoolComp);

			SpawnedActor->SetActorHiddenInGame(true);
			SpawnedActor->SetActorEnableCollision(false);
			SpawnedActor->SetActorTickEnabled(false);
			SpawnedActor->AttachToActor(PoolAnchor, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
		Pools.Add(SpawnParams.Name, CurrentPoolIndex);
	}
}

void UObjectPoolSubsystem::ClearPools()
{
	BroadcastPoolerCleanup();

	for (TPair<FName, FSingleObjectPool>& Pool : Pools)
	{
		for (int32 ObjectIndex = 0; ObjectIndex < Pool.Value.PooledObjects.Num(); ObjectIndex++)
		{
			if (IsValid(Pool.Value.PooledObjects[ObjectIndex]->GetOwner()))
			{
				Pool.Value.PooledObjects[ObjectIndex]->GetOwner()->Destroy();
			}
		}
	}

	PooledObjectData.Reset();
	Pools.Reset();
}

AActor* UObjectPoolSubsystem::GetPooledActor(const FName& PoolName)
{
	if (!Pools.Contains(PoolName))
	{
		return nullptr;
	}

	int32 PooledObjectCount = Pools[PoolName].PooledObjects.Num();

	int32 FirstAvailable = -1;

	for (int32 i = 0; i < PooledObjectCount; i++)
	{
		if (Pools[PoolName].PooledObjects[i] != nullptr)
		{
			if (!Pools[PoolName].PooledObjects[i]->bIsPoolActive)
			{
				FirstAvailable = i;
				break;
			}
		}
		else
		{
			RegenItem(PoolName, i);
			FirstAvailable = i;
			break;
		}
	}

	if (FirstAvailable >= 0)
	{
		UPooledObject* ToReturn = Pools[PoolName].PooledObjects[FirstAvailable];
		ToReturn->bIsPoolActive = true;

		OnPoolerCleanup.AddUniqueDynamic(ToReturn, &UPooledObject::RecycleSelf);

		AActor* ToReturnActor = ToReturn->GetOwner();

		ToReturnActor->SetActorHiddenInGame(false);
		ToReturnActor->SetActorEnableCollision(true);
		ToReturnActor->SetActorTickEnabled(true);
		ToReturnActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		return ToReturnActor;
	}

	if (!PooledObjectData[PoolName].bCanGrow)
	{
		return nullptr;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Name = PoolName;
	SpawnParams.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* SpawnedActor = GetWorld()->SpawnActor(PooledObjectData[PoolName].ActorTemplate, &FVector::ZeroVector, &FRotator::ZeroRotator, SpawnParams);

	SpawnedActor->SetActorLabel(SpawnedActor->GetName());

	UPooledObject* PoolComp = NewObject<UPooledObject>(SpawnedActor);

	PoolComp->RegisterComponent();
	SpawnedActor->AddInstanceComponent(PoolComp);

	PoolComp->Init(this);

	Pools[PoolName].PooledObjects.Add(PoolComp);

	PoolComp->bIsPoolActive = true;
	OnPoolerCleanup.AddUniqueDynamic(PoolComp, &UPooledObject::RecycleSelf);

	return SpawnedActor;
}

void UObjectPoolSubsystem::RecycleActor(AActor* PooledActor)
{
	if (UPooledObject* PoolCompRef = Cast<UPooledObject>(PooledActor->GetComponentByClass(UPooledObject::StaticClass())))
	{
		RecyclePooledObject(PoolCompRef);
	}
}

void UObjectPoolSubsystem::RecyclePooledObject(UPooledObject* PoolCompRef)
{
	OnPoolerCleanup.RemoveDynamic(PoolCompRef, &UPooledObject::RecycleSelf);

	PoolCompRef->bIsPoolActive = false;

	AActor* ReturningActor = PoolCompRef->GetOwner();
	ReturningActor->SetActorEnableCollision(false);
	ReturningActor->SetActorHiddenInGame(true);
	ReturningActor->SetActorTickEnabled(false);

	ReturningActor->AttachToActor(PoolAnchor, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void UObjectPoolSubsystem::BroadcastPoolerCleanup()
{
	OnPoolerCleanup.Broadcast();
}

void UObjectPoolSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UE_LOG(LogTemp, Log, TEXT("ObjectPoolSubsystem Initialized"));

	FWorldDelegates::OnPostWorldInitialization.AddUObject(this, &UObjectPoolSubsystem::OnWorldInitialized);
	FWorldDelegates::OnWorldCleanup.AddUObject(this, &UObjectPoolSubsystem::OnWorldCleanup);
}

void UObjectPoolSubsystem::Deinitialize()
{
	UE_LOG(LogTemp, Log, TEXT("ObjectPoolSubsystem Deinitialized"));

	FWorldDelegates::OnPostWorldInitialization.RemoveAll(this);
	FWorldDelegates::OnWorldCleanup.RemoveAll(this);
	Super::Deinitialize();
}

void UObjectPoolSubsystem::RegenItem(const FName& PoolName, int32 PositionIndex)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Name = PoolName;
	SpawnParams.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* SpawnedActor = GetWorld()->SpawnActor(PooledObjectData[PoolName].ActorTemplate, &FVector::ZeroVector, &FRotator::ZeroRotator, SpawnParams);
	SpawnedActor->SetActorLabel(SpawnedActor->GetName());
	UPooledObject* PoolComp = NewObject<UPooledObject>(SpawnedActor);
	PoolComp->RegisterComponent();
	SpawnedActor->AddInstanceComponent(PoolComp);

	PoolComp->Init(this);

	Pools[PoolName].PooledObjects.Insert(PoolComp, PositionIndex);

	SpawnedActor->SetActorHiddenInGame(true);
	SpawnedActor->SetActorEnableCollision(false);
	SpawnedActor->SetActorTickEnabled(false);
	SpawnedActor->AttachToActor(PoolAnchor, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void UObjectPoolSubsystem::OnWorldInitialized(UWorld* World, const UWorld::InitializationValues IValues)
{
	// 게임 월드인지 확인
	if (!World || !World->IsGameWorld()) return;

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

		PoolAnchor->SetActorLabel(TEXT("ObjectPoolAnchor"));
	}
}
void UObjectPoolSubsystem::OnWorldCleanup(UWorld* World, bool bSessionEnded, bool bCleanupResources)
{
	if (!World || !World->IsGameWorld()) return;

	// 풀 정리
	ClearPools();

	if (IsValid(PoolAnchor))
	{
		PoolAnchor->Destroy();
	}

	PoolAnchor = nullptr;
}