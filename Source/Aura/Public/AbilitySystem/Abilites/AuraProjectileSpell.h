// Copyright Jose Sotomayor

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilites/AuraDamageGameplayAbility.h"
#include "AuraProjectileSpell.generated.h"

/**
 * 
 */
class AAuraProjectile;
class UGameplayEffect;

UCLASS()
class AURA_API UAuraProjectileSpell : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UFUNCTION(BlueprintCallable, Category = "Events")

	
	void SpawnProjectile(const FVector& ProjectileTargetLocation);

	UPROPERTY(EditAnywhere, BlueprintReadonly)
	TSubclassOf<AAuraProjectile> ProjectileClass;

	
};
