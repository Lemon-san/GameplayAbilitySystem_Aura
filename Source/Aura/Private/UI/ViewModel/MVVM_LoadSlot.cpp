// Copyright Jose Sotomayor


#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadSlot::InitializeSlot()
{
	//TODO: CHECK SLOT STATUS BASED ON LOADED DATA
	const int32 WidgetSwitcherIndex = SlotStatus.GetValue();

	SetWidgetSwitcherIndexDelegate.Broadcast(WidgetSwitcherIndex);
}

void UMVVM_LoadSlot::SetPlayerName(FString NewPlayerName)
{
	UE_MVVM_SET_PROPERTY_VALUE(PlayerName, NewPlayerName);
}

void UMVVM_LoadSlot::SetLoadSlotName(FString NewLoadSlotName)
{
	UE_MVVM_SET_PROPERTY_VALUE(LoadSlotName, NewLoadSlotName);
}

void UMVVM_LoadSlot::SetMapName(FString NewMapName)
{
	UE_MVVM_SET_PROPERTY_VALUE(MapName, NewMapName);
}
