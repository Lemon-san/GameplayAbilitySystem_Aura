// Copyright Jose Sotomayor

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"


class UCameraComponent;
class USpringArmComponent;
class AAuraPlayerState;

/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()
	
public:
	AAuraCharacter();

	/**Initialize Ability Actor Info*/
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	/** End Initialize Ability Actor Info*/

	/** Combat Interface*/
	virtual int32 GetPlayerLevel() override;
	/**End CombatInterface */

	/**Player Interface*/
	virtual void AddToXP_Implementation(int32 InXP) override;
	/**End PlayerInterface*/

private:

	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TObjectPtr<USpringArmComponent> SpringArm;
};
