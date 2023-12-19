// Copyright Jose Sotomayor


#include "Actor/AuraEffectActor.h"
#include <AbilitySystemInterface.h>
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"


AAuraEffectActor::AAuraEffectActor()
{
 	
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));

}


void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();

}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetAbilitySystemComponent) return;

	check(GameplayEffectClass);

	FGameplayEffectContextHandle EffectContextHandle = TargetAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	const FGameplayEffectSpecHandle EffectSpecHandle = TargetAbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, 1.f, EffectContextHandle);
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;

	if (bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetAbilitySystemComponent);
	}
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if (EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		for (TSubclassOf<UGameplayEffect>& EffectClass : GameplayEffectClasses)
		{
			ApplyEffectToTarget(TargetActor, EffectClass);
		}
	}
	
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{

	if (EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		for (TSubclassOf<UGameplayEffect>& EffectClass : GameplayEffectClasses)
		{
			ApplyEffectToTarget(TargetActor, EffectClass);
		}
	}

	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetAbilitySystemComponent)) return;


		TArray<FActiveGameplayEffectHandle> HandlesToRemove;

		for (TPair<FActiveGameplayEffectHandle, UAbilitySystemComponent*> HandlePair : ActiveEffectHandles)
		{
			if (TargetAbilitySystemComponent == HandlePair.Value)
			{
				TargetAbilitySystemComponent->RemoveActiveGameplayEffect(HandlePair.Key, 1);
				HandlesToRemove.Add(HandlePair.Key);
			}
		}

		for (FActiveGameplayEffectHandle& Handle : HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}
}



