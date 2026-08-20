#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameplayTagContainer.h"
#include "Core/PoolTypes.h"
#include "OBGameModeBase.generated.h"

class AOBSpawnManager;

UCLASS()
class OUTBREAKBREAKER_API AOBGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AOBGameModeBase();

	FORCEINLINE AOBSpawnManager* GetSpawnManager() const { return SpawnManager; }

	void HandlePlayerVictory();

	void HandlePlayerDeath(AActor* Destroyer);

protected:
	virtual void BeginPlay() override;
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Outbreak | System")
	TSubclassOf<AOBSpawnManager> SpawnManagerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, FPooledObjectData> PooledObjectData;

	UPROPERTY(EditDefaultsOnly)
	int32 TargetMatchTimeInSeconds = 450;

private:
	UPROPERTY()
	TObjectPtr<AOBSpawnManager> SpawnManager = nullptr;
};
