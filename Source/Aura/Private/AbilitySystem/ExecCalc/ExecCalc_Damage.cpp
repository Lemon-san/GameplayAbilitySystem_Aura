// Copyright Jose Sotomayor


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Aura/Public/AuraGameplayTags.h"
#include <AbilitySystem/Data/CharacterClassInfo.h>
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include <Interaction/CombatInterface.h>
#include <AuraAbilityTypes.h>
#include "Kismet/GameplayStatics.h"

struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritHitResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritHitChance);

	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);

	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CritHitDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CritHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CritHitResistance, Target, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, FireResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, LightningResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArcaneResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, PhysicalResistance, Target, false);

	}
};

static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics DStatics;
	return DStatics;
}


UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().CritHitDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().CritHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CritHitResistanceDef);

	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDef;
	FillTagsToCapture(TagsToCaptureDef);

	//Boiler Plate
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	int32 SourcePlayerLevel = 1;
	if (SourceAvatar->Implements<UCombatInterface>())
	{
		SourcePlayerLevel = ICombatInterface::Execute_GetPlayerLevel(SourceAvatar);
	}

	int32 TargetPlayerLevel = 1;
	if (TargetAvatar->Implements<UCombatInterface>())
	{
		TargetPlayerLevel = ICombatInterface::Execute_GetPlayerLevel(TargetAvatar);
	}

	//ICombatInterface* SourceAvatarCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	//ICombatInterface* TargetAvatarCombatInterface = Cast<ICombatInterface>(TargetAvatar);

	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	FGameplayEffectContextHandle EffectContextHandle = EffectSpec.GetContext();
	FAuraGameplayTags AuraGameplayTags = FAuraGameplayTags::Get();

	//Debuffs
	DetermineDebuff(AuraGameplayTags, EffectSpec, ExecutionParams, EvaluationParameters, TagsToCaptureDef);

	//Get Damage Set by Caller Magnitude
	float Damage = 0.f;

	DetermineDamage(EffectContextHandle,SourceAvatar, TargetAvatar, AuraGameplayTags, EffectSpec, ExecutionParams, EvaluationParameters, TagsToCaptureDef, Damage);

	//Capture BlockChance on Target and Determine if there was a successful block
	float TargetBlockChance = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParameters, TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.f);

	//if Block, half the damage
	const bool bBlocked = FMath::RandRange(1, 100) < TargetBlockChance;
	UAuraAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bBlocked);

	Damage = bBlocked ? Damage * 0.5f : Damage;

	//Target Armor
	float TargetArmor = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters, TargetArmor);
	TargetArmor = FMath::Max<float>(TargetArmor, 0.f);


	//Source Coefficients

	const UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);

	const FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficient->FindCurve(FName("ArmorPenetration"), FString());
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourcePlayerLevel);

	const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficient->FindCurve(FName("EffectiveArmor"), FString());
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetPlayerLevel);

	const FRealCurve* CriticalHitResistanceCurve = CharacterClassInfo->DamageCalculationCoefficient->FindCurve(FName("CriticalHitResistance"), FString());
	const float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(TargetPlayerLevel);

	//Source Armor Penetration, ignores percentage of target armor
	float SourceArmorPen = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluationParameters, SourceArmorPen);
	SourceArmorPen = FMath::Max<float>(SourceArmorPen, 0.f);

	//Effective Armor
	const float EffectiveArmor = TargetArmor * (100 - SourceArmorPen * ArmorPenetrationCoefficient) / 100.f;
	Damage *= (100 - EffectiveArmor * EffectiveArmorCoefficient) / 100.f;

	//Critical Hit Damage
	float SourceCriticalHitDamage = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritHitDamageDef, EvaluationParameters, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(SourceCriticalHitDamage, 0.f);

	//Critical Hit Chance
	float SourceCritChance = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritHitChanceDef, EvaluationParameters, SourceCritChance);
	SourceCritChance = FMath::Max<float>(SourceCritChance, 0.f);

	// Critical Hit Resistance
	float TargetCritResistance = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritHitResistanceDef, EvaluationParameters, TargetCritResistance);
	TargetCritResistance = FMath::Max<float>(TargetCritResistance, 0.f);

	//Effective Critical Hit Resistance
	const float EffectiveCritChance = SourceCritChance - (TargetCritResistance * CriticalHitResistanceCoefficient);

	//is it a critical hit? update damage
	bool bCritical = FMath::RandRange(1, 100) < EffectiveCritChance;
	UAuraAbilitySystemLibrary::SetisCriticalHit(EffectContextHandle, bCritical);

	Damage = bCritical ? (Damage * 2.0f) + SourceCriticalHitDamage : Damage;


	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);

	OutExecutionOutput.AddOutputModifier(EvaluatedData);

}

void UExecCalc_Damage::FillTagsToCapture(TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& TagsToCaptureDef) const
{
	const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();

	TagsToCaptureDef.Add(Tags.Attributes_Secondary_Armor, DamageStatics().ArmorDef);
	TagsToCaptureDef.Add(Tags.Attributes_Secondary_BlockChance, DamageStatics().BlockChanceDef);
	TagsToCaptureDef.Add(Tags.Attributes_Secondary_ArmorPenetration, DamageStatics().ArmorPenetrationDef);
	TagsToCaptureDef.Add(Tags.Attributes_Secondary_CriticalHitDamage, DamageStatics().CritHitDamageDef);
	TagsToCaptureDef.Add(Tags.Attributes_Secondary_CriticalHitChance, DamageStatics().CritHitChanceDef);
	TagsToCaptureDef.Add(Tags.Attributes_Secondary_CriticalHitResistance, DamageStatics().CritHitResistanceDef);

	TagsToCaptureDef.Add(Tags.Attributes_Resistance_Fire, DamageStatics().FireResistanceDef);
	TagsToCaptureDef.Add(Tags.Attributes_Resistance_Lightning, DamageStatics().LightningResistanceDef);
	TagsToCaptureDef.Add(Tags.Attributes_Resistance_Arcane, DamageStatics().ArcaneResistanceDef);
	TagsToCaptureDef.Add(Tags.Attributes_Resistance_Physical, DamageStatics().PhysicalResistanceDef);
}

void UExecCalc_Damage::DetermineDamage(FGameplayEffectContextHandle& EffectContextHandle, AActor*& SourceAvatar, AActor*& TargetAvatar, FAuraGameplayTags& AuraGameplayTags, const FGameplayEffectSpec& EffectSpec, const FGameplayEffectCustomExecutionParameters& ExecutionParams, FAggregatorEvaluateParameters& EvaluationParameters, const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& TagsToCaptureDef, float& Damage) const
{
	for (const TPair<FGameplayTag, FGameplayTag>& PairTag : AuraGameplayTags.DamageTypesToResistances)
	{
		const FGameplayTag& DamageTypeTag = PairTag.Key;
		const FGameplayTag& DamageResistanceTag = PairTag.Value;

		checkf(TagsToCaptureDef.Contains(DamageResistanceTag), TEXT("TagsToCaptureDef doesn't contain tag: [%s] in ExecCalc_Damage"), *DamageResistanceTag.ToString());

		const FGameplayEffectAttributeCaptureDefinition CaptureDef = TagsToCaptureDef[DamageResistanceTag];

		float DamageTypeValue = EffectSpec.GetSetByCallerMagnitude(PairTag.Key, false);

		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluationParameters, Resistance);
		Resistance = FMath::Clamp(Resistance, 0.f, 100.f);

		DamageTypeValue *= (100.f - Resistance) / 100.f;

		if (UAuraAbilitySystemLibrary::IsRadialDamage(EffectContextHandle))
		{
			// 1. override TakeDamage in AuraCharacterBase. *DONE*
			// 2. Create Delegate OnDamageDelegate, broacast damage recieve in TakeDamage *DONE*
			// 3. Bind to OnDamageDelegate on the Victim here. *DONE*
			// 4. Call UGameplayStatics::ApplyRadialDamageWithFallOff to cause Damage 
			//    (this wil result in TakeDamage being called on the victim, which will broadcast OnDamageDelegate)
			// 5. In Lambda, set DamageTypeValue to the damage received from the broadcast *DONE*

			ICombatInterface* CombatInterface = Cast<ICombatInterface>(TargetAvatar);
			if (CombatInterface)
			{
				CombatInterface->GetOnDamageDelegate().AddLambda([&](float DamageAmount)
					{
						DamageTypeValue = DamageAmount;
					});
			}

			UGameplayStatics::ApplyRadialDamageWithFalloff(
				TargetAvatar,
				DamageTypeValue,
				0.f,
				UAuraAbilitySystemLibrary::GetRadialDamageOrigin(EffectContextHandle),
				UAuraAbilitySystemLibrary::GetRadialDamageInnerRadius(EffectContextHandle),
				UAuraAbilitySystemLibrary::GetRadialDamageOuterRadius(EffectContextHandle),
				1.f,
				UDamageType::StaticClass(),
				TArray<AActor*>(),
				SourceAvatar,
				nullptr);
		}

		Damage += DamageTypeValue;
	}
}

void UExecCalc_Damage::DetermineDebuff(FAuraGameplayTags& AuraGameplayTags, const FGameplayEffectSpec& EffectSpec, const FGameplayEffectCustomExecutionParameters& ExecutionParams, FAggregatorEvaluateParameters& EvaluationParameters, const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& TagsToCaptureDef) const
{
	for (const TTuple<FGameplayTag, FGameplayTag>& PairTag : AuraGameplayTags.DamageTypesToDebuffs)
	{
		const FGameplayTag& DamageTypeTag = PairTag.Key;
		const FGameplayTag& DebuffTypeTag = PairTag.Value;

		const float TypeDamage = EffectSpec.GetSetByCallerMagnitude(DamageTypeTag, false, -1.f);
		if (TypeDamage > -.5f) //.5 padding for floating point [im]precision (originally -1)
		{
			//Determine if there was a successful debuff
			const float SourceDebuffChance = EffectSpec.GetSetByCallerMagnitude(AuraGameplayTags.Debuff_Type_Chance, false, -1.f);

			float TargetDebuffResistance = 0.f;
			const FGameplayTag& ResistanceTag = AuraGameplayTags.DamageTypesToResistances[DamageTypeTag];
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(TagsToCaptureDef[ResistanceTag], EvaluationParameters, TargetDebuffResistance);

			TargetDebuffResistance = FMath::Max<float>(TargetDebuffResistance, 0.f);

			const float EffectiveDebuffChance = SourceDebuffChance * (100 - TargetDebuffResistance) / 100.f;
			const float RandomChance = FMath::RandRange(1, 100);
			const bool bDebuff = RandomChance < EffectiveDebuffChance;

			if (bDebuff)
			{

				FGameplayEffectContextHandle EffectContextHandle = EffectSpec.GetContext();

				const float DebuffDamage = EffectSpec.GetSetByCallerMagnitude(AuraGameplayTags.Debuff_Type_Damage, false, -1.f);
				const float DebuffDuration = EffectSpec.GetSetByCallerMagnitude(AuraGameplayTags.Debuff_Type_Duration, false, -1.f);
				const float DebuffFrequency = EffectSpec.GetSetByCallerMagnitude(AuraGameplayTags.Debuff_Type_Frequency, false, -1.f);
				const float DebuffStagger = EffectSpec.GetSetByCallerMagnitude(AuraGameplayTags.Debuff_Type_Stagger, false, -1.f);

				UAuraAbilitySystemLibrary::SetisSuccessfulDebuff(EffectContextHandle, true);
				UAuraAbilitySystemLibrary::SetDamageTypeTag(EffectContextHandle, DamageTypeTag);
				UAuraAbilitySystemLibrary::SetDebuffDamage(EffectContextHandle, DebuffDamage);
				UAuraAbilitySystemLibrary::SetDebuffDuration(EffectContextHandle, DebuffDuration);
				UAuraAbilitySystemLibrary::SetDebuffFrequency(EffectContextHandle, DebuffFrequency);

				if (DebuffStagger > 0.f)
				{
					UAuraAbilitySystemLibrary::SetisDebuffStagger(EffectContextHandle, true);
				}
			}
		}
	}
}
