// Copyright Jose Sotomayor

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"


class UOverlayWidgetController;
struct FWidgetControllerParams;
class UAuraUserWidget;
class UAbilitySystemComponent;
class UAttributeSet;
class APlayerState;
class APlayerController;

/**
 * 
 */
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()

public:

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

	UPROPERTY(VisibleAnywhere, Category = "Widget")
	TObjectPtr<UAuraUserWidget> OverlayWidget;

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "Widget")
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
	
public:

	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
};
