#include "Manager/OBSpawnManager.h"
#include "Kismet/GameplayStatics.h" 
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "NavigationSystem.h" 
#include "Core/ObjectPoolSubsystem.h"
#include "Interface/OBSpawnableInterface.h"

AOBSpawnManager::AOBSpawnManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AOBSpawnManager::BeginPlay()
{
	Super::BeginPlay();

	TargetPlayer = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (TargetPlayer)
	{
		UWorld* World = GetWorld();
		UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
		if (NavSys)
		{
			FVector PlayerLocation = TargetPlayer->GetActorLocation();
			while (PrecalculatedLocations.Num() < MaxLocationCacheSize)
			{
				float RandomAngle = FMath::FRandRange(0.0f, 360.0f);
				float RandomRadius = FMath::FRandRange(1300.0f, 1600.0f);
				float Radian = FMath::DegreesToRadians(RandomAngle);

				FVector TestLocation = PlayerLocation;
				TestLocation.X += FMath::Cos(Radian) * RandomRadius;
				TestLocation.Y += FMath::Sin(Radian) * RandomRadius;
				TestLocation.Z = PlayerLocation.Z + 20.0f;

				FNavLocation ProjectedLocation;
				if (NavSys->ProjectPointToNavigation(TestLocation, ProjectedLocation, FVector(500.0f, 500.0f, 500.0f)))
				{
					FVector ValidPos = ProjectedLocation.Location;
					ValidPos.Z += 20.0f;
					PrecalculatedLocations.Add(ValidPos);
				}
			}
		}
	}

	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AOBSpawnManager::CheckSpawnTimelineLoop, 1.0f, true);
}

void AOBSpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TargetPlayer && bIsTimelineActive)
	{
		TotalElapsedTime += DeltaTime;

		RotateAndRefreshSpawnLocationCache();
	}
}

void AOBSpawnManager::CheckSpawnTimelineLoop()
{
	if (!bIsTimelineActive || !TargetPlayer)
	{
		return;
	}

	if (SpawnDataTable)
	{
		TArray<FName> RowNames = SpawnDataTable->GetRowNames();

		for (const FName& RowName : RowNames)
		{
			FRuntimeSpawnRow* SpawnRow = SpawnDataTable->FindRow<FRuntimeSpawnRow>(RowName, TEXT(""));

			if (SpawnRow && SpawnRow->EnemyPoolTag.IsValid())
			{
				if (TotalElapsedTime >= SpawnRow->StartTime && TotalElapsedTime <= SpawnRow->EndTime)
				{
					if (SpawnRow->LastSpawnTime < 0.0f)
					{
						SpawnRow->LastSpawnTime = SpawnRow->StartTime;
					}

					if (TotalElapsedTime - SpawnRow->LastSpawnTime >= SpawnRow->SpawnInterval)
					{
						SpawnEnemyWave(SpawnRow->EnemyPoolTag, SpawnRow->SpawnAmount, SpawnRow->MonsterLevel);
						SpawnRow->LastSpawnTime = TotalElapsedTime;
					}
				}
			}
		}
	}
}

void AOBSpawnManager::SpawnEnemyWave(const FGameplayTag PoolTag, int32 Amount, int32 InLevel)
{
	if (!TargetPlayer || !PoolTag.IsValid() || PrecalculatedLocations.Num() == 0)
	{
		return;
	}

	for (int32 i = 0; i < Amount; ++i)
	{
		FVector FinalSpawnLocation = PrecalculatedLocations[FMath::RandRange(0, PrecalculatedLocations.Num() - 1)];

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(FinalSpawnLocation);
		SpawnTransform.SetRotation(FQuat::Identity);
		SpawnTransform.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		if (UObjectPoolSubsystem* PoolSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UObjectPoolSubsystem>())
		{
			AActor* EnemyActor = PoolSubsystem->GetPooledActor(PoolTag);
			EnemyActor->SetActorTransform(SpawnTransform, false, nullptr, ETeleportType::TeleportPhysics);
			if (IOBSpawnableInterface* Spawnable = Cast<IOBSpawnableInterface>(EnemyActor))
			{
				Spawnable->InitializeSpawnedObject(InLevel,TargetPlayer);
			}
		}

	}
}

void AOBSpawnManager::RotateAndRefreshSpawnLocationCache()
{
	if (!TargetPlayer || PrecalculatedLocations.Num() < MaxLocationCacheSize)
	{
		return;
	}

	UWorld* World = GetWorld();
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
	if (!NavSys)
	{
		return;
	}

	FVector PlayerLocation = TargetPlayer->GetActorLocation();

	float RandomAngle = FMath::FRandRange(0.0f, 360.0f);
	float RandomRadius = FMath::FRandRange(1300.0f, 1600.0f);
	float Radian = FMath::DegreesToRadians(RandomAngle);

	FVector TestLocation = PlayerLocation;
	TestLocation.X += FMath::Cos(Radian) * RandomRadius;
	TestLocation.Y += FMath::Sin(Radian) * RandomRadius;
	TestLocation.Z = PlayerLocation.Z + 20.0f;

	FNavLocation ProjectedLocation;
	if (NavSys->ProjectPointToNavigation(TestLocation, ProjectedLocation, FVector(500.0f, 500.0f, 500.0f)))
	{
		FVector ValidPos = ProjectedLocation.Location;
		ValidPos.Z += 20.0f;

		PrecalculatedLocations[CurrentRotationIndex] = ValidPos;

		CurrentRotationIndex = (CurrentRotationIndex + 1) % MaxLocationCacheSize;
	}
}

