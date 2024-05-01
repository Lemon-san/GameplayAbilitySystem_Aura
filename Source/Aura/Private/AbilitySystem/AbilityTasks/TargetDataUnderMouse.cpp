// Copyright Jose Sotomayor


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"
#include "AbilitySystemComponent.h"
#include "Aura/Aura.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);

	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	const bool bIsLocal = Ability->GetCurrentActorInfo()->IsLocallyControlled();

	if (bIsLocal)
	{
		SendMouseCursorData();
	}

	else
	{
		//We are on the server so listen for target data.
		const FGameplayAbilitySpecHandle SepcHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivtationPredictionkey = GetActivationPredictionKey();
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SepcHandle, ActivtationPredictionkey).AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallBack);
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SepcHandle, ActivtationPredictionkey);

		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}


}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	//Must create scope prediction window in order to do predictions
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());

	APlayerController* PlayerController = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorHit;
	PlayerController->GetHitResultUnderCursor(ECC_Target, false, CursorHit);

	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = CursorHit;

	FGameplayAbilityTargetDataHandle DataHandle; 
	DataHandle.Add(Data);

	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle, 
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey);

	//Must always have this before broadcast
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}

}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallBack(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());

	//Must always have this before broadcast
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
