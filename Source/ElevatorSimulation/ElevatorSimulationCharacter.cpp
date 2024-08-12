// Copyright Epic Games, Inc. All Rights Reserved.

#include "ElevatorSimulationCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Elevator/ElevatorControlPanel.h"
#include "Elevator/Elevator.h"

//////////////////////////////////////////////////////////////////////////
// AElevatorSimulationCharacter

AElevatorSimulationCharacter::AElevatorSimulationCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	Tags.Add("Player");
}

void AElevatorSimulationCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AElevatorSimulationCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		EnhancedInputComponent->BindAction(ElavatorInteractAction, ETriggerEvent::Started, this, &AElevatorSimulationCharacter::InteractControlPanel);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AElevatorSimulationCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AElevatorSimulationCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AElevatorSimulationCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AElevatorSimulationCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AElevatorSimulationCharacter::InteractControlPanel()
{
	if (m_CurrentElevatorControlPanel.IsValid()) {

		m_CurrentElevatorControlPanel->InteractControlPanel(this);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Invalid Interact Control Panel"));
	}
}

void AElevatorSimulationCharacter::SetElavatorControlPanel(AElevatorControlPanel* elevator)
{
	if (elevator)
		m_CurrentElevatorControlPanel = elevator;

}

void AElevatorSimulationCharacter::ClearElevatorControlPanel()
{
	m_CurrentElevatorControlPanel.Reset();
}

void AElevatorSimulationCharacter::ToggleShowMouseCursor(bool show)
{
	APlayerController* pc = Cast<APlayerController>(GetController());

	if (pc) {
		if (show) {
			pc->bShowMouseCursor = true;
			pc->SetInputMode(FInputModeGameAndUI());
		}
		else {
			pc->bShowMouseCursor = false;
			pc->SetInputMode(FInputModeGameOnly());
		}
	}

}

void AElevatorSimulationCharacter::TriggerSelectFloor_Server_Implementation(AElevator* elevator, int Destination)
{
	elevator->SetFloorDestination(Destination);
}

void AElevatorSimulationCharacter::TriggerCallElevator_Server_Implementation()
{
	m_CurrentElevatorControlPanel->CallElevator();
}

