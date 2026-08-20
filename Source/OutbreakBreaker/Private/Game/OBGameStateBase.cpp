#include "Game/OBGameStateBase.h"

AOBGameStateBase::AOBGameStateBase() {}

void AOBGameStateBase::StartMatchTimer(int32 InMaxMatchTime)
{
	MaxMatchTime = InMaxMatchTime;
	CurrentMatchTime = 0;

	OnMatchTimeChangedDelegate.Broadcast(CurrentMatchTime);

	GetWorldTimerManager().SetTimer(MatchTimerHandle, this, &AOBGameStateBase::UpdateTimer, 1.0f, true);
}

void AOBGameStateBase::UpdateTimer()
{
	CurrentMatchTime++;

	OnMatchTimeChangedDelegate.Broadcast(CurrentMatchTime);

	if (CurrentMatchTime >= MaxMatchTime)
	{
		GetWorldTimerManager().ClearTimer(MatchTimerHandle);
	}
}
