// Copyright Jose Sotomayor


#include "UI/WidgetController/AuraWidgetController.h"

void UAuraWidgetController::BroadcastInitialValues()
{
}

void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	AttributeSet = WCParams.AttributeSet;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	PlayerState = WCParams.PlayerState;
}

void UAuraWidgetController::BindCallbacksToDependencies()
{
}
