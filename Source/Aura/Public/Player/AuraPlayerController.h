// Copyright Jose Sotomayor

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "AuraPlayerController.generated.h"



class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UAuraInputConfig;
class UAuraAbilitySystemComponent;
class USplineComponent;
class UNiagaraSystem;
class UDamageTextComponent;
class AMagicCircle;
class IHighlightInterface;

UENUM(BlueprintType)
enum class ETargetingStatus : uint8
{
	TargetingEnemy,
	TargetingNonEnemy,
	NotTargeting
};
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

	UFUNCTION(BlueprintCallable)
	void ShowMagicCircle(UMaterialInterface* DecalMaterial = nullptr);

	UFUNCTION(BlueprintCallable)
	void HideMagicCircle();

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

	static void HightlightActor(AActor* InActor);
	static void UnHightlightActor(AActor* InActor);

	void ShiftPressed();
	void ShiftReleased();
	bool bShiftKeyDown = false;
	
	void AutoRun();

	void UpdateMagicCircleLocation();

	UAuraAbilitySystemComponent* GetAbilitySystemComponent();

	//TScriptInterface<IHighlightInterface> LastActor;
	//TScriptInterface<IHighlightInterface> CurrentActor;
	TObjectPtr<AActor> LastActor;
	TObjectPtr<AActor> CurrentActor;
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
	ETargetingStatus TargetingStatus = ETargetingStatus::NotTargeting;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	TObjectPtr<USplineComponent> Spline;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TObjectPtr<UNiagaraSystem> ClickNiagaraSystem;

	UPROPERTY(EditDefaultsOnly, Category = "MagicCircle")
	TSubclassOf<AMagicCircle> MagicCircleClass;

	UPROPERTY(VisibleDefaultsOnly, Category = "MagicCircle")
	TObjectPtr<AMagicCircle> MagicCircle;
};
