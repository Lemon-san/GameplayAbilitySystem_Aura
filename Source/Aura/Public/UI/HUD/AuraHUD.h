// Copyright Jose Sotomayor

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"


class UOverlayWidgetController;
class UAttributeMenuWidgetController;
struct FWidgetControllerParams;
class UAuraUserWidget;
class UAbilitySystemComponent;
class UAttributeSet;
class APlayerState;
class APlayerController;
class USpellMenuWidgetController;

/**
 * 
 */
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()

public:

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

protected:

	virtual void BeginPlay() override;

private:

	//OverlayWidget
	UPROPERTY(VisibleAnywhere, Category = "Widget")
	TObjectPtr<UAuraUserWidget> OverlayWidget;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "Widget")
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	//Attribute Menu Widget
	UPROPERTY(VisibleAnywhere, Category = "Widget")
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;

	//Spell Menu Widget
	UPROPERTY(VisibleAnywhere, Category = "Widget")
	TObjectPtr<USpellMenuWidgetController> SpellMenuWidgetController;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<USpellMenuWidgetController> SpellMenuWidgetControllerClass;
	
public:

	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams);
	USpellMenuWidgetController* GetSpellMenuWidgetController(const FWidgetControllerParams& WCParams);
};
