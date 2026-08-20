#include "Game/OBGameModeBase.h"
#include "Manager/OBSpawnManager.h"
#include "Core/ObjectPoolSubsystem.h"
#include "GameFramework/PlayerState.h"
#include "Game/OBGameStateBase.h"
#include "Character/OBCharacter.h"
#include "UI/HUD/OBHUD.h"

AOBGameModeBase::AOBGameModeBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AOBGameModeBase::HandlePlayerVictory()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->SetPause(true);
	}

	if (AOBHUD* OBHUD = Cast<AOBHUD>(PC->GetHUD()))
	{
		OBHUD->ShowVictoryScreen();
	}
}

void AOBGameModeBase::HandlePlayerDeath(AActor* Destroyer)
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->SetPause(true);
	}

	if (AOBHUD* OBHUD = Cast<AOBHUD>(PC->GetHUD()))
	{
		OBHUD->ShowGameOverScreen();
	}
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

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		AOBCharacter* Player = Cast<AOBCharacter>(PC->GetPawn());
		if (Player)
		{
			Player->OnCharacterDeadDelegate.AddUObject(this, &AOBGameModeBase::HandlePlayerDeath);
		}
	}

	AOBGameStateBase* OBGameState = GetGameState<AOBGameStateBase>();
	if (OBGameState)
	{
		OBGameState->StartMatchTimer(TargetMatchTimeInSeconds);
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
