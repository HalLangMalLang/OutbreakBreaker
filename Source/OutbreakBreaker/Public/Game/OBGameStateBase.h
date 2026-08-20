#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "OBGameStateBase.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMatchTimeChangedSignature, int32 /* CurrentTimeInSeconds */);

UCLASS()
class OUTBREAKBREAKER_API AOBGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	AOBGameStateBase();

	void StartMatchTimer(int32 InMaxMatchTime);

	FORCEINLINE int32 GetCurrentMatchTime() const { return CurrentMatchTime; }

protected:
	void UpdateTimer();

public:
	FOnMatchTimeChangedSignature OnMatchTimeChangedDelegate;

private:
	int32 CurrentMatchTime = 0;

	int32 MaxMatchTime = 0;

	FTimerHandle MatchTimerHandle;
};
