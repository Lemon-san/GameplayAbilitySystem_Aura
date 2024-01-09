// Copyright Jose Sotomayor

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "AuraPlayerController.generated.h"


class UInputMappingContext;
class UInputAction;
class IEnemyInterface;
struct FInputActionValue;
class UAuraInputConfig;
class UAuraAbilitySystemComponent;

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

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UAuraAbilitySystemComponent* GetAbilitySystemComponent();

	IEnemyInterface* LastActor;
	IEnemyInterface* CurrentActor;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	TObjectPtr<UAuraInputConfig> InputConfig;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

};
