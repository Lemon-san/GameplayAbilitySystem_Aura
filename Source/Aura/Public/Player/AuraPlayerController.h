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
class USplineComponent;

class UDamageTextComponent;
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

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockHit, bool bCriticalHit);

protected:
	virtual void BeginPlay() override;

private:
	
	void Move(const FInputActionValue& InputActionValue);

	void CursorTrace();

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	void ShiftPressed();
	void ShiftReleased();
	bool bShiftKeyDown = false;
	
	void AutoRun();


	UAuraAbilitySystemComponent* GetAbilitySystemComponent();

	IEnemyInterface* LastActor;
	IEnemyInterface* CurrentActor;
	FHitResult CursorHit;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	TObjectPtr<UInputAction> ShiftAction;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	TObjectPtr<UAuraInputConfig> InputConfig;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	//Clicking to Move
	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
	bool bTargetting = false;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	TObjectPtr<USplineComponent> Spline;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

};
