// Copyright Jose Sotomayor

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilites/AuraDamageGameplayAbility.h"
#include "ArcaneShards.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UArcaneShards : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

public:

	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 NextLevel) override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 MaxNumShards = 11;
	
};
