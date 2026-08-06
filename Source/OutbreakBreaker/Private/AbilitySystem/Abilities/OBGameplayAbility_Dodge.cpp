#include "AbilitySystem/Abilities/OBGameplayAbility_Dodge.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/OBCharacter.h"

void UOBGameplayAbility_Dodge::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
        this,
        TEXT("DodgeMontageTask"),
        DodgingMontage,
        1.0f
    );

    if (MontageTask)
    {
        if (AOBCharacter* OBCharacter = Cast<AOBCharacter>(ActorInfo->AvatarActor.Get()))
        {
            OBCharacter->DodgingLaunch();
        }

        MontageTask->OnCompleted.AddDynamic(this, &UOBGameplayAbility_Dodge::OnDodgeMontageFinished);
        MontageTask->OnInterrupted.AddDynamic(this, &UOBGameplayAbility_Dodge::OnDodgeMontageFinished);
        MontageTask->OnCancelled.AddDynamic(this, &UOBGameplayAbility_Dodge::OnDodgeMontageFinished);

        MontageTask->ReadyForActivation();
    }
}

void UOBGameplayAbility_Dodge::OnDodgeMontageFinished()
{
    if (CurrentActorInfo)
    {
        if (AOBCharacter* OBCharacter = Cast<AOBCharacter>(CurrentActorInfo->AvatarActor.Get()))
        {
            OBCharacter->StopDodgingMovement();
        }
    }

    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
