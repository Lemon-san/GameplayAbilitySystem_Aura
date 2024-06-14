// Copyright Jose Sotomayor

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadScreen.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSlotSelectedSignature);

class UMVVM_LoadSlot;
/**
 * 
 */
UCLASS()
class AURA_API UMVVM_LoadScreen : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:

	void InitializeLoadSlots();

	void LoadData();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadSlot> LoadSlotViewModelClass;

	UFUNCTION(BlueprintCallable)
	void DeleteButtonPressed();

	UFUNCTION(BlueprintCallable)
	void NewSlotButtonPressed(int32 Slot, const FString& EnterName);

	UFUNCTION(BlueprintCallable)
	void NewGameButtonPressed(int32 Slot);

	UFUNCTION(BlueprintCallable)
	void SelectSlotButtonPressed(int32 Slot);

	UFUNCTION(BlueprintCallable)
	void PlayButtonPressed();

	/*Delegates*/
	UPROPERTY(BlueprintAssignable)
	FSlotSelectedSignature SlotSelectedDelegate;

	/*Field Notifies*/
	void SetNumLoadSlots(int32 NewLoadSlots);

	int32 GetNumLoadSlots() const { return NumLoadSlots; }

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	int32 NumLoadSlots;

	UPROPERTY()
	TMap<int32, UMVVM_LoadSlot*> LoadSlots;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_0;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_1;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_2;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UMVVM_LoadSlot> SelectedSlot;

public:

	UFUNCTION(BlueprintPure)
	UMVVM_LoadSlot* GetLoadSlotViewModelByIndex(int32 Index) const;
};
