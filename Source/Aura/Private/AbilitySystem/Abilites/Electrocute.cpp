// Copyright Jose Sotomayor


#include "AbilitySystem/Abilites/Electrocute.h"

FString UElectrocute::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);

	if (Level == 1)
	{
		return FString::Printf(TEXT(
			//Title
			"<Title>ELECTROCUTE</>\n\n"
			//Level
			"<Small>Level: </><Level>%d</>\n"
			//ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			//Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			//Number of Shock Targets
			"<Default>Emits a beam of lightning, "
			"connecting with the target, repeatedly causing: </>"
			//Damage
			"<Damage>%d</><Default> lightning damage with a chance to </>"
			"<Burn>Stun</>\n\n"
		),
			//Values
			Level, ManaCost, Cooldown, ScaledDamage);
	}

	else
	{
		return FString::Printf(TEXT(
			//Title
			"<Title>ELECTROCUTE</>\n\n"
			//Level
			"<Small>Level: </><Level>%d</>\n"
			//ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			//Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			//Number of Additional Shock Targets
			"<Default>Emits a beam of lightning, propagating to %d additional targets nearby, dealing: </>"
			//Damage
			"<Damage>%d</><Default> lightning damage with a chance to </>"
			"<Burn>Stun.</>\n\n"
		),
			//Values
			Level, ManaCost, Cooldown, FMath::Min(Level, MaxNumShockTargets - 1), ScaledDamage);
	}
}

FString UElectrocute::GetNextLevelDescription(int32 NextLevel)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(NextLevel);
	const float ManaCost = FMath::Abs(GetManaCost(NextLevel));
	const float Cooldown = GetCooldown(NextLevel);

	return FString::Printf(TEXT(
		//Title
		"<Title>NEXT LEVEL</>\n\n"
		//Level
		"<Small>Level: </><Level>%d</>\n"
		//ManaCost
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		//Cooldown
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
		//Bolts Of Fire
		"<Default>Emits a beam of lightning, propagating to %d additional targets nearby and dealing:  "
		//Damage
		"</><Damage>%d</><Default> lightning damage with a chance to </>"
		"<Burn>Stun.</>\n\n"
	),
		//Values
		NextLevel, ManaCost, Cooldown, FMath::Min(NextLevel, MaxNumShockTargets - 1), ScaledDamage);
}
