// Copyright Jose Sotomayor


#include "UI/WidgetController/SpellMenuWidgetController.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerState.h"
#include "AuraGameplayTags.h"

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	const int32 SpellPoints = GetAuraPS()->GetSpellPoints();

	FGameplayTag AbilityStatusTag;

	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTag(GameplayTags.Abilities_None);

	const FGameplayAbilitySpec* AbilitySpec = GetAuraASC()->GetSpecFromAbilityTag(AbilityTag);
	const bool bSpecValid = AbilitySpec != nullptr;

	if (!bTagValid || bTagNone || !bSpecValid)
	{
		AbilityStatusTag = GameplayTags.Abilities_Status_Locked;
	}

	else
	{
		AbilityStatusTag = GetAuraASC()->GetStatusFromSpec(*AbilitySpec);
	}

	SelectedAbility.Ability = AbilityTag;
	SelectedAbility.Status = AbilityStatusTag;

	bool bEnableSpellPoints = false;
	bool bEnableEquip = false;

	FString Description;
	FString NextLevelDescription;

	GetAuraASC()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);

	ShouldEnableButtons(AbilityStatusTag, SpellPoints, bEnableSpellPoints, bEnableEquip);

	SpellGlobeSelectedDelegate.Broadcast(bEnableSpellPoints, bEnableEquip, Description, NextLevelDescription);

}

void USpellMenuWidgetController::GlobeDeselect()
{
	SelectedAbility.Ability = FAuraGameplayTags::Get().Abilities_None;
	SelectedAbility.Status = FAuraGameplayTags::Get().Abilities_Status_Locked;

	SpellGlobeSelectedDelegate.Broadcast(false, false, FString(), FString());
}

void USpellMenuWidgetController::SpendPointButtonPressed()
{

	if (GetAuraASC())
	{
		GetAuraASC()->ServerSpendSpellPoint(SelectedAbility.Ability);
	}
	
}



void USpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();

	SpellPointsChangedDelegate.Broadcast(GetAuraPS()->GetSpellPoints());

}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	GetAuraASC()->AbilityStatusChangedDelegate.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 NewSpellLevel)
		{
			if (SelectedAbility.Ability.MatchesTagExact(AbilityTag))
			{
				SelectedAbility.Status = StatusTag;

				bool bEnableSpellPoints = false;
				bool bEnableEquip = false;
				FString Description;
				FString NextLevelDescription;

				GetAuraASC()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);

				ShouldEnableButtons(StatusTag, CurrentSpellPoints, bEnableSpellPoints, bEnableEquip);
				SpellGlobeSelectedDelegate.Broadcast(bEnableSpellPoints, bEnableEquip, Description, NextLevelDescription);

			}

			if (AbilityInfo)
			{
				FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
				Info.StatusTag = StatusTag;
				AbilityInfoDelegate.Broadcast(Info);
			}
		});

	GetAuraPS()->OnSpellPointChangeDelegate.AddLambda([this](int32 SpellPoints) 
		{
			SpellPointsChangedDelegate.Broadcast(SpellPoints);
			CurrentSpellPoints = SpellPoints;

			bool bEnableSpellPoints = false;
			bool bEnableEquip = false;
			FString Description;
			FString NextLevelDescription;

			GetAuraASC()->GetDescriptionsByAbilityTag(SelectedAbility.Ability, Description, NextLevelDescription);

			ShouldEnableButtons(SelectedAbility.Status, CurrentSpellPoints, bEnableSpellPoints, bEnableEquip);
			SpellGlobeSelectedDelegate.Broadcast(bEnableSpellPoints, bEnableEquip,Description, NextLevelDescription);
		});
}

void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SpellPoints, bool& bShouldEnableSpellPointsButton, bool& bShouldEnableEquipButton)
{
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();

	bShouldEnableSpellPointsButton = false;
	bShouldEnableEquipButton = false;

	if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
	{
		bShouldEnableEquipButton = true;

		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
	{

		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
	{
		bShouldEnableEquipButton = true;

		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}

	}
}
