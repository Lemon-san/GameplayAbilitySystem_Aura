// Copyright Jose Sotomayor


#include "Player/AuraPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Interaction/EnemyInterface.h"
#include "Aura/DebugMacros.h"



AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);

}

// Called to bind functionality to input
void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	
	//Finds out which direction is forward using controller as base.
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	//get forward vector of the controller
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	//get Right Vector of the controller
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		//adds the movement input forward vector * the direction
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		//adds the movement input right vector * the direction
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}

}


void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, CursorHit);

	if (!CursorHit.bBlockingHit) return;

	LastActor = CurrentActor;
	CurrentActor =	Cast<IEnemyInterface>(CursorHit.GetActor());

	/*
	Line trace from cursor, there are several scenarios:
	A. LastActor is null && CurrentActor is null
		-Do Nothing
	B. LastActor is null && CurrentActor is valid
		-Highlight current actor
	C. LastActor is valid && CurrentActor is null
		-Unhighlight last actor
	D. LastActor is valid && CurrentActor is valid but LastActor != CurrentActor
		-Unhighlight Last Actor and Highlight CurrentActor
	E. Both actors are valid, and are the same actor
		-Do nothing
	*/

	if (!LastActor)
	{
		if (CurrentActor)
		{
			//Case B
			CurrentActor->HighlightActor();
			
		}

		else
		{
			//Case A, both are null do nothing
		}
	}

	else //Last Actor is valid
	{
		if (!CurrentActor)
		{
			//Case C
			LastActor->UnHighlightActor();
		}

		else
		{
			//Case D, both actors are valid
			if (LastActor != CurrentActor)
			{
				LastActor->UnHighlightActor();
				CurrentActor->HighlightActor();
			}
			else
			{
				//Case E - do nothing
			}
		}
	}

}
