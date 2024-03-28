// Copyright Jose Sotomayor

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "AuraPlayerState.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChange, int32 /*StatValue*/);

/**
 * 
 */

class UAbilitySystemComponent;
class UAttributeSet;
class ULevelUpInfo;

UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAuraPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void AddTolevel(int32 InLevel);
	void SetLevel(int32 InLevel);

	void AddToXP(int32 InXP);
	void SetXP(int32 InXP);

	void AddToAttributePoints(int32 InAttributePoints);

	void AddToSpellPoints(int32 InSpellPoints);

	FOnPlayerStatChange OnXPChangedDelegate;
	FOnPlayerStatChange OnLevelChangeDelegate;
	FOnPlayerStatChange OnAttributePointChangeDelegate;
	FOnPlayerStatChange OnSpellPointChangeDelegate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UAttributeSet> AttributeSet;

private:

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level, Category = "Level")
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_XP, Category = "Level")
	int32 XP = 1;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_AttributePoints, Category = "Level")
	int32 AttributePoints = 0;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_SpellPoints, Category = "Level")
	int32 SpellPoints = 1;

	UFUNCTION()
	void OnRep_Level(const int32& OldLevel) const;

	UFUNCTION()
	void OnRep_XP(const int32& OldXP) const;

	UFUNCTION()
	void OnRep_AttributePoints(const int32& OldAttributePoints) const;

	UFUNCTION()
	void OnRep_SpellPoints(const int32& OldSpellPoints) const;


public:

	FORCEINLINE UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	FORCEINLINE int32 GetPlayerLevel() const { return Level; }
	FORCEINLINE int32 GetPlayerXP() const { return XP; }
	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }
	FORCEINLINE int32 GetSpellPoints() const { return SpellPoints; }
};
