// Copyright Jose Sotomayor


#include "AbilitySystem/Abilites/AuraFireBlast.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraFireBall.h"

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

	TArray<AAuraFireBall*> Fireballs;

	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const FVector Axis = FVector::UpVector;

	TArray<FRotator> EvenlySpacedRotators = UAuraAbilitySystemLibrary::EvenlySpacedRotators(
		Forward,
		Axis,
		360.f,
		MaxNumFireBalls);

	for (const FRotator& Rotator : EvenlySpacedRotators)
	{

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(Location);
		SpawnTransform.SetRotation(Rotator.Quaternion());

		AAuraFireBall* Fireball = GetWorld()->SpawnActorDeferred<AAuraFireBall>(
			FireBallClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			CurrentActorInfo->PlayerController->GetPawn(),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		Fireball->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		Fireball->ReturnToActor = GetAvatarActorFromActorInfo();
		Fireball->SetOwner(GetAvatarActorFromActorInfo());

		FDamageEffectParams ExplosionDamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		Fireball->ExplosionDamageParams = ExplosionDamageEffectParams;

		//Fireball->SetOwner(GetAvatarActorFromActorInfo());
		Fireballs.Add(Fireball);
		Fireball->FinishSpawning(SpawnTransform);
	}
	return Fireballs;
}
