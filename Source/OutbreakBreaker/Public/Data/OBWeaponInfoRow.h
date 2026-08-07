#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "OBWeaponInfoRow.generated.h"

/**
 * 주무기 3종 및 보조무기 3종의 모든 밸런싱 수치를 한눈에 관리하는 마스터 데이터 테이블 행 구조체
 */
USTRUCT(BlueprintType)
struct OUTBREAKBREAKER_API FOBWeaponInfoRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	FOBWeaponInfoRow() {}

	// [주무기 1] 플라즈마 라이플 (Plasma Rifle) 전용 스탯
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Plasma Rifle")
	float PlasmaRifleDamage = 0.f; // 틱당 기본 데미지
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Plasma Rifle")
	float TickInterval = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Plasma Rifle")
	float PassiveRange = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Plasma Rifle")
	int32 PenetrationCount = 0;

	// [주무기 2] 그라비티 해머 (Gravity Hammer) 전용 스탯
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Gravity Hammer")
	float GravityHammerDamage = 0.f; // 타격 데미지
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Gravity Hammer")
	float SlamCooldown = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Gravity Hammer")
	float KnockbackForce = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Gravity Hammer")
	float PullRadius = 0.f;

	// [주무기 3] 스프레드 샷건 (Spread Shotgun) 전용 스탯
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Spread Shotgun")
	float DamagePerPellet = 0.f; // 산탄 한 알당 데미지
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Spread Shotgun")
	float PelletCount = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Spread Shotgun")
	float SpreadAngle = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Spread Shotgun")
	float FireCooldown = 0.f;

	// [보조무기 1] 디펜스 드론 (Defense Drone) 전용 스탯
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Defense Drone")
	float PushForce = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Defense Drone")
	float OrbitRadius = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Defense Drone")
	float DeflectCooldown = 0.f;

	// [보조무기 2] 마그넷 마인 (Magnet Mine) 전용 스탯
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Magnet Mine")
	float ExplosionDamage = 0.f; // 폭발 데미지
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Magnet Mine")
	float ExplosionRadius = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Magnet Mine")
	float SlowAmount = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Magnet Mine")
	float DropInterval = 0.f;

	// [보조무기 3] 아우라 모듈 (Aura Module) 전용 스탯
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Aura Module")
	float TickDamage = 0.f; // 틱당 지속 데미지
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Aura Module")
	float AuraRadius = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Aura Module")
	float AuraTickInterval = 0.f;
};