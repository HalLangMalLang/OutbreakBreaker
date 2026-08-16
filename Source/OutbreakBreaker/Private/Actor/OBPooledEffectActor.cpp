#include "Actor/OBPooledEffectActor.h"
#include "Components/SphereComponent.h"
#include "Core/ObjectPoolSubsystem.h"
#include "Components/StaticMeshComponent.h"
#include "GameplayEffect.h"

AOBPooledEffectActor::AOBPooledEffectActor()
{
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	ItemMesh->SetCollisionProfileName(TEXT("NoCollision"));
	RootComponent = ItemMesh;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetCollisionProfileName(TEXT("PickupItemDetector"));
	SphereComponent->SetupAttachment(ItemMesh);
}

void AOBPooledEffectActor::OnSpawnFromPool()
{
	SphereComponent->SetCollisionProfileName(TEXT("PickupItemDetector"));

	ReceiveOnSpawnFromPool();
}

void AOBPooledEffectActor::OnRecycleToPool()
{
	SphereComponent->SetCollisionProfileName(TEXT("PoolStorage"));

	ReceiveOnRecycleToPool();
}

void AOBPooledEffectActor::InitializeSpawnedObject(int32 InLevel, AActor* InTargetPlayer)
{
	EffectMagnitude = InLevel;

	TargetPlayer = InTargetPlayer;
}

void AOBPooledEffectActor::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AOBPooledEffectActor::OnItemOverlap);
}

void AOBPooledEffectActor::OnItemOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* CurrentOwner = GetOwner();
	if (!OtherActor || OtherActor == CurrentOwner || OtherActor == this)
	{
		return;
	}

	if (InstantGameplayEffectClass || DurationGameplayEffectClass || InfiniteGameplayEffectClass)
	{
		OnOverlap(OtherActor, CurrentOwner, SetByCallerTag, EffectMagnitude);
	}

	if (UObjectPoolSubsystem* PoolSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UObjectPoolSubsystem>())
	{
		PoolSubsystem->RecycleActor(this);
	}
}
