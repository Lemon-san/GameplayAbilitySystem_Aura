// Copyright Jose Sotomayor

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include <Interaction/HighlightInterface.h>
#include "Interaction/SaveInterface.h"
#include <Aura/Aura.h>
#include "AuraCheckpoint.generated.h"

class USphereComponent;
/**
 * 
 */
UCLASS()
class AURA_API AAuraCheckpoint : public APlayerStart, public ISaveInterface, public IHighlightInterface
{
	GENERATED_BODY()
	
public:

	AAuraCheckpoint(const FObjectInitializer& ObjectInitializer);

	/*Save Interface */
	virtual bool ShouldLoadTransform_Implementation() override;
	virtual void LoadActor_Implementation() override;
	/*End Save interface*/

	/*Highlight Interface*/
	virtual void SetMoveToLocation_Implementation(FVector& OutDestination) override;
	virtual void HighlightActor_Implementation() override;
	virtual void UnHighlightActor_Implementation() override;
	/*End Hightlight Interface*/

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> MoveToComponent;

	UPROPERTY(BlueprintReadWrite, SaveGame)
	bool bReached = false;

	UPROPERTY(EditAnywhere)
	bool bBindOverlapCallback = true;
	

protected:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void HandleGlowEffect();

	int32 CustomDepthStencilOverride = CUSTOM_DEPTH_TAN;

	UFUNCTION(BlueprintImplementableEvent)
	void CheckpointReached(UMaterialInstanceDynamic* DynamicMaterialInstance);

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> CheckpointMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> Sphere;


};
