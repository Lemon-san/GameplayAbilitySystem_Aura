// Copyright Jose Sotomayor

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "AuraPlayerState.generated.h"

/**
 * 
 */

class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAuraPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UAttributeSet> AttributeSet;

private:

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level, Category = "Level")
	int32 Level = 1;

	UFUNCTION()
	void OnRep_Level(const int32& OldLevel) const;


public:

	FORCEINLINE UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	FORCEINLINE int32 GetPlayerLevel() const { return Level; }
};
