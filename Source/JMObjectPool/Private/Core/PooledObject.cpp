#include "Core/PooledObject.h"
#include "Core/ObjectPoolSubsystem.h"

void UPooledObject::Init(UObjectPoolSubsystem* Owner)
{
	bIsPoolActive = false;

	ObjectPool = Owner;
}

void UPooledObject::RecycleSelf()
{
	ObjectPool->RecyclePooledObject(this);
}

void UPooledObject::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	ObjectPool->OnPoolerCleanup.RemoveDynamic(this, &UPooledObject::RecycleSelf);

	Super::OnComponentDestroyed(bDestroyingHierarchy);
}
