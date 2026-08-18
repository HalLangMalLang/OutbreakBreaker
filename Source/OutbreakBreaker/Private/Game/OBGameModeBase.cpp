#include "Game/OBGameModeBase.h"
#include "Manager/OBSpawnManager.h"
#include "Core/ObjectPoolSubsystem.h"

AOBGameModeBase::AOBGameModeBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AOBGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World && SpawnManagerClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Owner = this;

		SpawnManager = World->SpawnActor<AOBSpawnManager>(SpawnManagerClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	}
}

void AOBGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	if (!PooledObjectData.IsEmpty())
	{
		if (UObjectPoolSubsystem* PoolSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UObjectPoolSubsystem>())
		{
			PoolSubsystem->SetupPools(PooledObjectData);
			UE_LOG(LogTemp, Log, TEXT("[GameMode] %d개의 태그 풀 데이터를 성공적으로 초기화"), PooledObjectData.Num());
		}
	}
}
