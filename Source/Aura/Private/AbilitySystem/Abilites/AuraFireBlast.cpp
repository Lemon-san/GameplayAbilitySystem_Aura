// Copyright Jose Sotomayor


#include "AbilitySystem/Abilites/AuraFireBlast.h"

FString UAuraFireBlast::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);

	return FString::Printf(TEXT(
		//Title
		"<Title>FIRE BLAST</>\n\n"
		//Level
		"<Small>Level: </><Level>%d</>\n"
		//ManaCost
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		//Cooldown
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
		//Bolts of Fire
		"<Default>Launches %d "
		"fire balls in all directions, each coming back and exploding causing: </>"
		//Damage
		"<Damage>%d</><Default> radial fire damage with a chance to </>"
		"<Burn>burn</>\n\n"
	),
		//Values
		Level, ManaCost, Cooldown, MaxNumFireBalls, ScaledDamage);

}
	

FString UAuraFireBlast::GetNextLevelDescription(int32 NextLevel)
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
		//Bolts of Fire
		"<Default>Launches %d "
		"fire balls in all directions, each coming back and exploding causing: </>"
		//Damage
		"<Damage>%d</><Default> radial fire damage with a chance to </>"
		"<Burn>burn</>\n\n"
	),
		//Values
		NextLevel, ManaCost, Cooldown, MaxNumFireBalls, ScaledDamage);
}

TArray<AAuraFireBall*> UAuraFireBlast::SpawnFireBalls()
{
	return TArray<AAuraFireBall*>();
}
