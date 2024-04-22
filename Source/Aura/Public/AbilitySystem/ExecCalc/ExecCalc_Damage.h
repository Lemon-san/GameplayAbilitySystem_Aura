// Copyright Jose Sotomayor

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecCalc_Damage.generated.h"

struct FAuraGameplayTags;
struct FGameplayEffectSpec;
/**
 * 
 */
UCLASS()
class AURA_API UExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:

	UExecCalc_Damage();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

	void FillTagsToCapture(TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& TagsToCaptureDef) const;

	void DetermineDamage(FAuraGameplayTags& AuraGameplayTags, const FGameplayEffectSpec& EffectSpec, const FGameplayEffectCustomExecutionParameters& ExecutionParams, FAggregatorEvaluateParameters& EvaluationParameters, const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& TagsToCaptureDef, float& Damage) const;

	void DetermineDebuff(FAuraGameplayTags& AuraGameplayTags, const FGameplayEffectSpec& EffectSpec, const FGameplayEffectCustomExecutionParameters& ExecutionParams, FAggregatorEvaluateParameters& EvaluationParameters, const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& TagsToCaptureDef) const;
	
};
