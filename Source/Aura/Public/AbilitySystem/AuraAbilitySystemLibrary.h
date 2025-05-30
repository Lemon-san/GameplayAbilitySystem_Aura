// Copyright Jose Sotomayor

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/CharacterClassInfo.h"
#include "AuraAbilitySystemLibrary.generated.h"


class UOverlayWidgetController;
class UAttributeMenuWidgetController;
class UAbilitySystemComponent;
class USpellMenuWidgetController;
struct FWidgetontrollerParams;
class UAbilityInfo;
class ULootTiersInfo;
class ULoadScreenSaveGame;

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	/*
	 *Widget Controller
	*/
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary | WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWcParams, AAuraHUD*& OutAuraHUD);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary | WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary | WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary | WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContextObject);

	/*
	 *Ability System Class Defaults
	*/

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | Defaults")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | Defaults")
	static void InitializeDefaultAttributesFromSaveData(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ULoadScreenSaveGame* SaveGame);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | Defaults")
	static void GiveStartupAbbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | Defaults")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | Defaults")
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | Defaults", meta = (DefaultToSelf = "WorldContextObject"))
	static ULootTiersInfo* GetLootTiers(const UObject* WorldContextObject);


	/*
	 * Effect Context Getters
	*/

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static bool IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static float GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static float GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static float GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static FGameplayTag GetDamageTypeTag(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static FVector GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static FVector GetKnockbackForce(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static bool IsDebuffStagger(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static bool IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static float GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static float GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static FVector GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle);

	/*
	 * Effect Context Setters
	*/

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static void SetIsBlockedHit(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static void SetisCriticalHit(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static void SetisSuccessfulDebuff(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool bInIsSuccessfulDebuff);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static void SetisDebuffStagger(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool bInIsDebuffStagger);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static void SetDebuffDamage(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, float NewDebuffDamage);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static void SetDebuffDuration(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, float NewDebuffDuration);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static void SetDebuffFrequency(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, float NewDebuffFrequency);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static void SetDamageTypeTag(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& NewDamageTypeTag);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static void SetDeathImpulse(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, const FVector& NewDeathImpulse);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static void SetKnockbackForce(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, const FVector& NewKnockbackForce);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static void SetIsRadialDamage(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool bNewIsRadialDamage);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static void SetRadialDamageInnerRadius(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, float NewRadialDamageInnerRadius);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static void SetRadialDamageOuterRadius(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, float NewRadialDamageOuterRadius);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static void SetRadialDamageOrigin(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, const FVector& NewRadialDamageOrigin);
	/*
	 *Gameplay Mechanic Algorithms	
	*/

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | GameplayMechanics")
	static void GetAllLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | GameplayMechanics")
	static void GetClosestTargets(int32 MaxTargets, const TArray<AActor*>& Actors, TArray<AActor*>& OutClosestTargets, const FVector& Origin);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary | GameplayMechanics")
	static bool IsNotFriend(AActor* FirstActor, AActor* SecondActor);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | GameplayMechanics")
	static int32 GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | DamageEffect")
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary | GameplayMechanics")
	static TArray<FRotator> EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumRotators);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary | GameplayMechanics")
	static TArray<FVector> EvenlyRotatedVectors(const FVector& Forward, const FVector& Axis, float Spread, int32 NumVectors);

	/*
	*Damage Effect Params
	*/

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | DamageEffectParam")
	static void SetIsRadialDamageEffectParam(UPARAM(ref)FDamageEffectParams& DamageEffectParams, bool bIsRadial, float InnerRadius, float OuterRadius, FVector Origin);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | DamageEffectParam")
	static void SetKnockbackDirectionDamageEffectParam(UPARAM(ref)FDamageEffectParams& DamageEffectParams, FVector KnockbackDirection, float Magnitude = 0.f);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | DamageEffectParam")
	static void SetDeathImpulseDirectionDamageEffectParam(UPARAM(ref)FDamageEffectParams& DamageEffectParams, FVector DeathImpulseDirection, float Magnitude = 0.f);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | DamageEffectParam")
	static void SetTargetEffectParamsASC(UPARAM(ref)FDamageEffectParams& DamageEffectParams, UAbilitySystemComponent* InASC);
};
