// Copyright Jose Sotomayor

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilites/AuraGameplayAbility.h"
#include <Interaction/CombatInterface.h>
#include "AuraDamageGameplayAbility.generated.h"


/**
 * 
 */
UCLASS()
class AURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;

	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;

	float GetDamageByDamageType(float Inlevel, const FGameplayTag& DamageType);
	
};
