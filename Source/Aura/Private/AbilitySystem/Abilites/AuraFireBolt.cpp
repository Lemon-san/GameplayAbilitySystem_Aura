// Copyright Jose Sotomayor


#include "AbilitySystem/Abilites/AuraFireBolt.h"
#include "Aura/Public/AuraGameplayTags.h"

FString UAuraFireBolt::GetDescription(int32 Level)
{
	
	const int32 Damage = GetDamageByDamageType(Level, FAuraGameplayTags::Get().Damage_Fire);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);

	if (Level == 1)
	{
		return FString::Printf(TEXT(
			//Title
			"<Title>FIRE BOLT</>\n\n"
			//Level
			"<Small>Level: </><Level>%d</>\n"
			//ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			//Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			//Bolts of Fire
			"<Default>Launches a bolt of fire, "
			"exploding on impact and dealing: </>"
			//Damage
			"<Damage>%d</><Default> fire damage with a chance to </>"
			"<Burn>burn</>\n\n"
			),
			//Values
			Level,ManaCost,Cooldown,Damage);
	}

	else
	{
		return FString::Printf(TEXT(
			//TiTle
			"<Title>FIRE BOLT</>\n\n"
			//Level
			"<Small>Level: </><Level>%d</>\n"
			//ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			//Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			//Bolts of Fire
			"<Default>Launches %d bolts of fire, exploding on impact and dealing: </>"
			//Damage
			"<Damage>%d</><Default> fire damage with a chance to </>"
			"<Burn>burn</>\n\n"
			),
			//Values
			Level, ManaCost,Cooldown, FMath::Min(Level, NumberOfProjectiles), Damage);
	}
}

FString UAuraFireBolt::GetNextLevelDescription(int32 NextLevel)
{
	const int32 Damage = GetDamageByDamageType(NextLevel, FAuraGameplayTags::Get().Damage_Fire);
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
		"<Default>Launches %d bolts of fire, exploding on impact and dealing: "
		//Damage
		"</><Damage>%d</><Default> fire damage with a chance to </>"
		"<Burn>burn</>\n\n"
		),
		//Values
		NextLevel, ManaCost,Cooldown, FMath::Min(NextLevel, NumberOfProjectiles), Damage);
}