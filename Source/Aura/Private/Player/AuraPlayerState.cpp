// Copyright Jose Sotomayor


#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include <Net/UnrealNetwork.h>

AAuraPlayerState::AAuraPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	// how often the server will try to update the client
	NetUpdateFrequency = 100.f;
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAuraPlayerState, Level);
	DOREPLIFETIME(AAuraPlayerState, XP);
	DOREPLIFETIME(AAuraPlayerState, AttributePoints);
	DOREPLIFETIME(AAuraPlayerState, SpellPoints);
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraPlayerState::AddTolevel(int32 InLevel)
{
	Level += InLevel;
	OnLevelChangeDelegate.Broadcast(Level);
}

void AAuraPlayerState::SetLevel(int32 InLevel)
{
	Level = InLevel;
	OnLevelChangeDelegate.Broadcast(Level);
}

void AAuraPlayerState::AddToXP(int32 InXP)
{
	XP += InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void AAuraPlayerState::SetXP(int32 InXP)
{
	XP = InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void AAuraPlayerState::AddToAttributePoints(int32 InAttributePoints)
{
	AttributePoints += InAttributePoints;
	OnAttributePointChangeDelegate.Broadcast(AttributePoints);
}

void AAuraPlayerState::AddToSpellPoints(int32 InSpellPoints)
{
	SpellPoints += InSpellPoints;
	OnSpellPointChangeDelegate.Broadcast(SpellPoints);
}

void AAuraPlayerState::OnRep_Level(const int32& OldLevel) const
{
	OnLevelChangeDelegate.Broadcast(Level);
}

void AAuraPlayerState::OnRep_XP(const int32& OldXP) const
{
	OnXPChangedDelegate.Broadcast(XP);
}

void AAuraPlayerState::OnRep_AttributePoints(const int32& OldAttributePoints) const
{
	OnAttributePointChangeDelegate.Broadcast(AttributePoints);
}

void AAuraPlayerState::OnRep_SpellPoints(const int32& OldSpellPoints) const
{
	OnSpellPointChangeDelegate.Broadcast(SpellPoints);
}
