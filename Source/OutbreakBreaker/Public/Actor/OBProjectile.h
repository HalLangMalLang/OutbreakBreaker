#pragma once

#include "CoreMinimal.h"
#include "Actor/OBEffectActor.h"
#include "OBProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class OUTBREAKBREAKER_API AOBProjectile : public AOBEffectActor
{
	GENERATED_BODY()
	
public:
	AOBProjectile();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile | Collision")
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile | Movement")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
};
