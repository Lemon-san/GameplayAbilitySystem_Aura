// Copyright Jose Sotomayor

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"


class UCameraComponent;
class USpringArmComponent;
class AAuraPlayerState;

/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()
	
public:
	AAuraCharacter();

	/**Initialize Ability Actor Info*/
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	/** End Initialize Ability Actor Info*/

private:

	void InitAbilityActorInfo();

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TObjectPtr<USpringArmComponent> SpringArm;
};
