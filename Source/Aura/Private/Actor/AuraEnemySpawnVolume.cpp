// Copyright Jose Sotomayor


#include "Actor/AuraEnemySpawnVolume.h"
#include <Components/BoxComponent.h>
#include <Interaction/PlayerInterface.h>
#include "Actor/AuraEnemySpawnPoint.h"


AAuraEnemySpawnVolume::AAuraEnemySpawnVolume()
{

	PrimaryActorTick.bCanEverTick = false;

	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	SetRootComponent(Box);
	Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Box->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	Box->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

}

void AAuraEnemySpawnVolume::LoadActor_Implementation()
{
	if (bReached)
	{
		Destroy();
	}
}


void AAuraEnemySpawnVolume::BeginPlay()
{
	Super::BeginPlay();

	Box->OnComponentBeginOverlap.AddDynamic(this, &AAuraEnemySpawnVolume::OnBoxOverlap);
	
}

void AAuraEnemySpawnVolume::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->Implements<UPlayerInterface>())
	{
		return;
	}
	bReached = true;

	for (AAuraEnemySpawnPoint* SpawnPoint : SpawnPoints)
	{
		if (IsValid(SpawnPoint))
		{
			SpawnPoint->SpawnEnemy();
		}
	}

	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

