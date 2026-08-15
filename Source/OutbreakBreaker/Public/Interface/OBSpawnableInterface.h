#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OBSpawnableInterface.generated.h"

UINTERFACE(MinimalAPI)
class UOBSpawnableInterface : public UInterface
{
	GENERATED_BODY()
};


class OUTBREAKBREAKER_API IOBSpawnableInterface
{
	GENERATED_BODY()

public:
	virtual void InitializeSpawnedObject(int32 InLevel, AActor* InTargetPlayer) = 0;
};
