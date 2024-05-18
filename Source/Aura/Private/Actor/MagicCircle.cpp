// Copyright Jose Sotomayor


#include "Actor/MagicCircle.h"
#include "Components/DecalComponent.h"

// Sets default values
AMagicCircle::AMagicCircle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MagicCircleDecalComponent = CreateDefaultSubobject<UDecalComponent>("MagicCircleDecal");
	MagicCircleDecalComponent->SetupAttachment(GetRootComponent());

}

void AMagicCircle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called when the game starts or when spawned
void AMagicCircle::BeginPlay()
{
	Super::BeginPlay();
	
}

