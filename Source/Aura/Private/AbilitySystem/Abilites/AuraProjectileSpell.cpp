// Copyright Jose Sotomayor


#include "AbilitySystem/Abilites/AuraProjectileSpell.h"
#include <Interaction/CombatInterface.h>
#include "AbilitySystemBlueprintLibrary.h"
#include "Actor/AuraProjectile.h"
#include "AbilitySystemComponent.h"
#include "Aura/Public/AuraGameplayTags.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{

	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	//ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	//if (CombatInterface)
	//{
		const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), FAuraGameplayTags::Get().CombatSocket_Weapon);
		//const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();

		//Set the Projectile Rotation
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());

	

		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);


		//Give the Projectile a gamepaly effect spec for causing damage.
		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());

		FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();

		EffectContextHandle.SetAbility(this);
		EffectContextHandle.AddSourceObject(Projectile);
		TArray<TWeakObjectPtr<AActor>> Actors;
		Actors.Add(Projectile);
		EffectContextHandle.AddActors(Actors);
		FHitResult HitResult;
		HitResult.Location = ProjectileTargetLocation;
		EffectContextHandle.AddHitResult(HitResult);

		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);

		//Set by caller only : Key value pair, key is damage, value is magnitude 
		const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();

		for (TPair<FGameplayTag, FScalableFloat>& Pair : DamageTypes)
		{
			const float ScaleDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaleDamage);
			
		}
		
		
		
		Projectile->DamageEffectSpecHandle = SpecHandle; 

		Projectile->FinishSpawning(SpawnTransform);
	//}
}
