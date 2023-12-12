// Copyright Jose Sotomayor

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"


class UInputMappingContext;
class UInputAction;
class IEnemyInterface;
struct FInputActionValue;
/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AAuraPlayerController();

	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	
	void Move(const FInputActionValue& InputActionValue);

	void CursorTrace();

	TObjectPtr<IEnemyInterface> LastActor;
	TObjectPtr<IEnemyInterface> CurrentActor;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	TObjectPtr<UInputAction> MoveAction;

};
