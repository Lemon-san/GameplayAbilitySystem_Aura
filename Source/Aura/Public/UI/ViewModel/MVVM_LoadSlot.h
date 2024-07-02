// Copyright Jose Sotomayor

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Game/LoadScreenSaveGame.h"
#include "MVVM_LoadSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndexSignature, int32, WidgetSwitcherIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnableSelectSlotButtonSignature, bool, bEnable);

/**
 * 
 */
UCLASS()
class AURA_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:

	void InitializeSlot();

	UPROPERTY()
	FName PlayerStartTag;

	UPROPERTY()
	int32 SlotIndex;

	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SlotStatus;

	UPROPERTY()
	FString MapAssetName;

	//Delegates

	UPROPERTY(BlueprintAssignable)
	FSetWidgetSwitcherIndexSignature SetWidgetSwitcherIndexDelegate;

	UPROPERTY(BlueprintAssignable)
	FEnableSelectSlotButtonSignature EnableSelectSlotButtonDelegate;


	/*Field Notifies*/

	void SetPlayerName(FString NewPlayerName);
	FString GetPlayerName() const { return PlayerName;  }

	void SetLoadSlotName(FString NewLoadSlotName);
	FString GetLoadSlotName() const { return LoadSlotName; }

	void SetMapName(FString NewMapName);
	FString GetMapName() const { return MapName; }

	void SetPlayerLevel(int32 NewPlayerLevel);
	int32 GetPlayerLevel() const { return PlayerLevel; }

private:

	/*Field Notifies*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess="true"))
	FString PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	FString MapName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	int32 PlayerLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess="true"))
	FString LoadSlotName;
};
