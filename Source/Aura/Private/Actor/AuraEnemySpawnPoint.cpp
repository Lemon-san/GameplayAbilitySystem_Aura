// Copyright Jose Sotomayor


#include "Actor/AuraEnemySpawnPoint.h"
#include "Character/AuraEnemy.h"

void AAuraEnemySpawnPoint::SpawnEnemy()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AAuraEnemy* Enemy = GetWorld()->SpawnActorDeferred<AAuraEnemy>(EnemyClass, GetActorTransform());
	if (Enemy)
	{
		Enemy->SetEnemyLevel(EnemyLevel);
		Enemy->SetCharacterClass(CharacterClass);
		Enemy->FinishSpawning(GetActorTransform());
		Enemy->SpawnDefaultController();
	}
}
