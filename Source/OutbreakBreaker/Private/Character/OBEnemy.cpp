#include "Character/OBEnemy.h"
#include "AbilitySystem/OBAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/OBEnemyAttributeSet.h"

AOBEnemy::AOBEnemy()
{
	AbilitySystemComponent = CreateDefaultSubobject<UOBAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UOBEnemyAttributeSet>(TEXT("AttributeSet"));
}

void AOBEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();
}

void AOBEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	InitializeDefaultAttributes();
	InitializeAttributeDelegates();

	AddCharacterAbilities();
}

void AOBEnemy::InitializeAttributeDelegates()
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		if (const UOBEnemyAttributeSet* OBAS = Cast<UOBEnemyAttributeSet>(AttributeSet))
		{
			const_cast<UOBEnemyAttributeSet*>(OBAS)->OnEnemyDeadDelegate.AddUObject(this, &AOBEnemy::OnCharacterDeathProcessed);
			const_cast<UOBEnemyAttributeSet*>(OBAS)->OnSpeedEnemyDelegate.AddUObject(this, &AOBEnemy::OnMoveSpeedVelocityUpdated);

			OBAS->OnSpeedEnemyDelegate.Broadcast(OBAS->GetMoveSpeed());
		}
	}
}

void AOBEnemy::OnCharacterDeathProcessed(AActor* Destroyer)
{

}

