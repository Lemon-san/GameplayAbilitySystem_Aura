// Copyright Jose Sotomayor


#include "AbilitySystem/Abilites/ArcaneShards.h"

FString UArcaneShards::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);

	if (Level == 1)
	{
		return FString::Printf(TEXT(
			//Title
			"<Title>ARCANE SHARDS</>\n\n"
			//Level
			"<Small>Level: </><Level>%d</>\n"
			//ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			//Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			//Number of Shards shummoned
			"<Default>Summon a shard of arcane energy, "
			"causing radial arcane damage of: </>"
			//Damage
			"<Damage>%d</><Default> damage at the shard origin. </>"
			"\n\n"
		),
			//Values
			Level, ManaCost, Cooldown, ScaledDamage);
	}

	else
	{
		return FString::Printf(TEXT(
			//Title
			"<Title>ARCANE SHARDS</>\n\n"
			//Level
			"<Small>Level: </><Level>%d</>\n"
			//ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			//Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			//Number of Shards summoned
			"<Default>Summons %d shards of arcane energy, causing radial arcane damage of : </>"
			//Damage
			"<Damage>%d</><Default> damage at the shard origins.</>"
			"\n\n"
		),
			//Values
			Level, ManaCost, Cooldown, FMath::Min(Level, MaxNumShards), ScaledDamage);
	}
}

FString UArcaneShards::GetNextLevelDescription(int32 NextLevel)
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
		//Number of Shards summoned
		"<Default>Summons %d shards of arcane energy, causing radial arcane damage of : </>"
		//Damage
		"<Damage>%d</><Default> damage at the shard origins.</>"
		"\n\n"
	),
		//Values
		NextLevel, ManaCost, Cooldown, FMath::Min(NextLevel, MaxNumShards), ScaledDamage);
}
