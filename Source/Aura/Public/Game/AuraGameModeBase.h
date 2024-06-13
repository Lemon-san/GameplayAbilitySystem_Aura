// Copyright Jose Sotomayor

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AuraGameModeBase.generated.h"

class UCharacterClassInfo;
class UAbilityInfo;
class UMVVM_LoadSlot;
class USaveGame;
class ULoadScreenSaveGame;

/**
 * 
 */
UCLASS()
class AURA_API AAuraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Maps")
	TSoftObjectPtr<UWorld> DefaultMap;

	UPROPERTY(EditDefaultsOnly, Category = "Maps")
	TMap<FString, TSoftObjectPtr<UWorld>> Maps;

	UPROPERTY(EditDefaultsOnly, Category = "Maps")
	FString DefaultMapName;

	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Ability Info")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USaveGame> LoadScreenSaveGameClass;

	void SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex);

	static void DeleteSlot(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex);

	ULoadScreenSaveGame* GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const;

protected:

	virtual void BeginPlay() override;
	
	
};
