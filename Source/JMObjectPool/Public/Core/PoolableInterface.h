#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PoolableInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPoolableInterface : public UInterface
{
	GENERATED_BODY()
};


class JMOBJECTPOOL_API IPoolableInterface
{
	GENERATED_BODY()

public:
	virtual void OnSpawnFromPool() = 0;
	virtual void OnRecycleToPool() = 0;
};
