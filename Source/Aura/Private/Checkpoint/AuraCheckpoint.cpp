// Copyright Jose Sotomayor


#include "Checkpoint/AuraCheckpoint.h"
#include "Components/SphereComponent.h"
#include <Interaction/PlayerInterface.h>
#include <Game/AuraGameModeBase.h>
#include <Kismet/GameplayStatics.h>


AAuraCheckpoint::AAuraCheckpoint(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

	PrimaryActorTick.bCanEverTick = false;

	CheckpointMesh = CreateDefaultSubobject<UStaticMeshComponent>("CheckpointMesh");
	CheckpointMesh->SetupAttachment(GetRootComponent());
	CheckpointMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CheckpointMesh->SetCollisionResponseToAllChannels(ECR_Block);
	CheckpointMesh->SetCustomDepthStencilValue(CustomDepthStencilOverride);
	CheckpointMesh->MarkRenderStateDirty();

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(CheckpointMesh);

	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	MoveToComponent = CreateDefaultSubobject<USceneComponent>("MoveToComponent");
	MoveToComponent->SetupAttachment(GetRootComponent());


}

bool AAuraCheckpoint::ShouldLoadTransform_Implementation()
{
	return false;
}

void AAuraCheckpoint::LoadActor_Implementation()
{
	if (bReached)
	{
		HandleGlowEffect();
	}
}

void AAuraCheckpoint::SetMoveToLocation_Implementation(FVector& OutDestination)
{
	OutDestination = MoveToComponent->GetComponentLocation();
}

void AAuraCheckpoint::HighlightActor_Implementation()
{
	if (!bReached)
	{
		CheckpointMesh->SetRenderCustomDepth(true);
	}
	
}

void AAuraCheckpoint::UnHighlightActor_Implementation()
{
	CheckpointMesh->SetRenderCustomDepth(false);
}

void AAuraCheckpoint::BeginPlay()
{
	Super::BeginPlay();

	if (bBindOverlapCallback)
	{
		Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraCheckpoint::OnSphereOverlap);
	}
	
}

void AAuraCheckpoint::HandleGlowEffect()
{
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	UMaterialInstanceDynamic* DynamicMaterialInstance = UMaterialInstanceDynamic::Create(CheckpointMesh->GetMaterial(0),this);

	CheckpointMesh->SetMaterial(0, DynamicMaterialInstance);

	CheckpointReached(DynamicMaterialInstance);
}

void AAuraCheckpoint::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UPlayerInterface>())
	{
		bReached = true;
		AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
		if (AuraGM)
		{
			const UWorld* World = GetWorld();
			FString MapName = World->GetMapName();
			MapName.RemoveFromStart(World->StreamingLevelsPrefix);


			AuraGM->SaveWorldState(GetWorld(), MapName);
		}

		IPlayerInterface::Execute_SaveProgress(OtherActor, PlayerStartTag);

		HandleGlowEffect();
	}
}
