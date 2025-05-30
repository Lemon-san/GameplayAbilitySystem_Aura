// Copyright Jose Sotomayor

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilites/AuraBeamSpell.h"
#include "Electrocute.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UElectrocute : public UAuraBeamSpell
{
	GENERATED_BODY()

public:

	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 NextLevel) override;
	
};
