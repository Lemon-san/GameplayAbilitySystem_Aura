// Copyright Jose Sotomayor

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AuraGameModeBase.generated.h"

class UCharacterClassInfo;
class UAbilityInfo;
class ULootTiersInfo;
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

	void TravelToMap(UMVVM_LoadSlot* Slot);

	void SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex);

	static void DeleteSlot(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex);

	ULoadScreenSaveGame* GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	ULoadScreenSaveGame* RetrieveInGameSaveData();

	void SaveInGameProgressData(ULoadScreenSaveGame* SaveObject);

	void SaveWorldState(UWorld* World, const FString& DestinationMapAssetName = FString("")) const;
	void LoadWorldState(UWorld* World) const;

	void PlayerDied(ACharacter* DeadCharacter);

	UPROPERTY(EditDefaultsOnly, Category = "Maps")
	TSoftObjectPtr<UWorld> DefaultMap;

	UPROPERTY(EditDefaultsOnly, Category = "Maps")
	TMap<FString, TSoftObjectPtr<UWorld>> Maps;

	UPROPERTY(EditDefaultsOnly, Category = "Maps")
	FString DefaultMapName;

	UPROPERTY(EditDefaultsOnly, Category = "Maps")
	FName DefaultPlayerStartTag;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USaveGame> LoadScreenSaveGameClass;

	/*Data Assets*/
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Ability Info")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	UPROPERTY(EditDefaultsOnly, Category = "LootTiers Info")
	TObjectPtr<ULootTiersInfo> LootTiersInfo;

	FString GetMapNameFromMapAssetName(const FString& MapAssetName) const;

protected:

	virtual void BeginPlay() override;
	
	
};
