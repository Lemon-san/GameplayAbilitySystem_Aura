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
struct FGameplayTag;

UCLASS()
class AURA_API UAuraProjectileSpell : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()



protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UFUNCTION(BlueprintCallable, Category = "Events")
	void SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch = false, float PitchOverride = 0.f);

	UPROPERTY(EditAnywhere, BlueprintReadonly)
	TSubclassOf<AAuraProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadonly)
	int32 NumberOfProjectiles = 5;
	
};
