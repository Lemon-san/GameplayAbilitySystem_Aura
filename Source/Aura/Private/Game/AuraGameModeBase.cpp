// Copyright Jose Sotomayor


#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"
#include <Game/LoadScreenSaveGame.h>
#include "GameFramework/PlayerStart.h"
#include <Game/AuraGameInstance.h>
#include <EngineUtils.h>
#include <Interaction/SaveInterface.h>
#include <Serialization/ObjectAndNameAsStringProxyArchive.h>
#include "Aura/AuraLogChannels.h"
#include "GameFramework/Character.h"


void AAuraGameModeBase::TravelToMap(UMVVM_LoadSlot* Slot)
{
	const FString SlotName = Slot->GetLoadSlotName();
	const int32 SlotIndex = Slot->SlotIndex;
	TSoftObjectPtr<UWorld> Level = Maps.FindChecked(Slot->GetMapName());

	UGameplayStatics::OpenLevelBySoftObjectPtr(Slot, Level);
}

void AAuraGameModeBase::SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex)
{
	//Checks if LoadSlot has same name and index, if so deletes it
	DeleteSlot(LoadSlot, SlotIndex);

	//If not, then create a new save game
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameObject);
	LoadScreenSaveGame->PlayerName = LoadSlot->GetPlayerName();
	LoadScreenSaveGame->MapName = LoadSlot->GetMapName();
	LoadScreenSaveGame->SaveSlotStatus = Taken;
	LoadScreenSaveGame->PlayerStartTag = LoadSlot->PlayerStartTag;
	LoadScreenSaveGame->MapAssetName = LoadSlot->MapAssetName;
	
	UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame, LoadSlot->GetLoadSlotName(), SlotIndex);
}

void AAuraGameModeBase::DeleteSlot(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(LoadSlot->GetLoadSlotName(), SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlot->GetLoadSlotName(), SlotIndex);
	}
}

ULoadScreenSaveGame* AAuraGameModeBase::GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const
{
	USaveGame* SaveGameObject = nullptr;

	bool SaveExists = UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex);
	if (SaveExists)
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex);
	}

	else
	{
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	}

	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameObject);
	return LoadScreenSaveGame;
}

AActor* AAuraGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());

	TArray<AActor*> OutActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), OutActors);

	if (OutActors.Num() > 0)
	{
		AActor* SelectedActor = OutActors[0];
		for (AActor* Actor : OutActors)
		{
			if (APlayerStart* PlayerStart = Cast<APlayerStart>(Actor))
			{
				if (PlayerStart->PlayerStartTag == AuraGameInstance->PlayerStartTag)
				{
					SelectedActor = PlayerStart;
					break;
				}
			}
		}
		return SelectedActor;
	}

	return nullptr;
}

ULoadScreenSaveGame* AAuraGameModeBase::RetrieveInGameSaveData()
{
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());

	if (AuraGameInstance)
	{
		const FString InGameLoadSlotName = AuraGameInstance->LoadSlotName;
		const int32 InGameLoadSlotIndex = AuraGameInstance->LoadSlotIndex;

		return GetSaveSlotData(InGameLoadSlotName, InGameLoadSlotIndex);
	}

	return nullptr;
}

void AAuraGameModeBase::SaveInGameProgressData(ULoadScreenSaveGame* SaveObject)
{
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());

	if (AuraGameInstance)
	{
		const FString InGameLoadSlotName = AuraGameInstance->LoadSlotName;
		const int32 InGameLoadSlotIndex = AuraGameInstance->LoadSlotIndex;
		AuraGameInstance->PlayerStartTag = SaveObject->PlayerStartTag;

		UGameplayStatics::SaveGameToSlot(SaveObject, InGameLoadSlotName, InGameLoadSlotIndex);
	}
}

void AAuraGameModeBase::SaveWorldState(UWorld* World, const FString& DestinationMapAssetName) const
{
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);

	//Save All The Actors in the world that have the SaveGameInterface Implemented
	UAuraGameInstance* AuraGI = Cast<UAuraGameInstance>(GetGameInstance());
	check(AuraGI);

	ULoadScreenSaveGame* SaveGame = GetSaveSlotData(AuraGI->LoadSlotName, AuraGI->LoadSlotIndex);
	if (SaveGame)
	{
		if (DestinationMapAssetName != (""))
		{
			SaveGame->MapAssetName = DestinationMapAssetName;
			SaveGame->MapName = GetMapNameFromMapAssetName(DestinationMapAssetName);

		}

		if (!SaveGame->HasMap(WorldName))
		{
			FSavedMap NewSaveMap;
			NewSaveMap.MapAssetName = WorldName;
			SaveGame->SavedMaps.Add(NewSaveMap);
		}

		FSavedMap SavedMap = SaveGame->GetSavedMapWithName(WorldName);
		SavedMap.SavedActors.Empty(); //Clear out Array, we will fill it with "actors"

		for (FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;

			if (!IsValid(Actor) || !Actor->Implements<USaveInterface>()) continue;

			FSavedActor SavedActor;
			SavedActor.ActorName = Actor->GetFName();
			SavedActor.Transform = Actor->GetTransform();

			//Passing Actors as Bytes
			FMemoryWriter MemoryWriter(SavedActor.Bytes);
			FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
			Archive.ArIsSaveGame = true;

			Actor->Serialize(Archive);
			SavedMap.SavedActors.AddUnique(SavedActor);
		}

		for (FSavedMap& MapToReplace : SaveGame->SavedMaps)
		{
			if (MapToReplace.MapAssetName == WorldName)
			{
				MapToReplace = SavedMap;
			}
		}

		UGameplayStatics::SaveGameToSlot(SaveGame, AuraGI->LoadSlotName, AuraGI->LoadSlotIndex);

	}
}

void AAuraGameModeBase::LoadWorldState(UWorld* World) const
{
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);

	UAuraGameInstance* AuraGI = Cast<UAuraGameInstance>(GetGameInstance());
	check(AuraGI);

	if (UGameplayStatics::DoesSaveGameExist(AuraGI->LoadSlotName, AuraGI->LoadSlotIndex))
	{
		ULoadScreenSaveGame* SaveGame = Cast<ULoadScreenSaveGame>(UGameplayStatics::LoadGameFromSlot(AuraGI->LoadSlotName, AuraGI->LoadSlotIndex));
		
		if (!SaveGame)
		{
			UE_LOG(LogAura, Error, TEXT("Failed to load Slot"));
			return;
		}
		for (FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;

			if (!Actor->Implements<USaveInterface>()) continue;

			for (FSavedActor SavedActor : SaveGame->GetSavedMapWithName(WorldName).SavedActors)
			{
				if (SavedActor.ActorName == Actor->GetFName())
				{
					if (ISaveInterface::Execute_ShouldLoadTransform(Actor))
					{
						Actor->SetActorTransform(SavedActor.Transform);
					}

					FMemoryReader MemoryReader(SavedActor.Bytes);

					FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
					Archive.ArIsSaveGame = true;
					Actor->Serialize(Archive); // Deserializes->converts binary bytes back into variables.

					ISaveInterface::Execute_LoadActor(Actor);
				}
			}
		}
	}
}

void AAuraGameModeBase::PlayerDied(ACharacter* DeadCharacter)
{
	ULoadScreenSaveGame* SaveGame = RetrieveInGameSaveData();
	if (!IsValid(SaveGame)) return;

	UGameplayStatics::OpenLevel(DeadCharacter, FName(SaveGame->MapAssetName));
}

FString AAuraGameModeBase::GetMapNameFromMapAssetName(const FString& MapAssetName) const
{
	for (auto& Map : Maps)
	{
		if (Map.Value.ToSoftObjectPath().GetAssetName() == MapAssetName)
		{
			return Map.Key;
		}
	}
	return FString();
}

void AAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	Maps.Add(DefaultMapName, DefaultMap);
}
