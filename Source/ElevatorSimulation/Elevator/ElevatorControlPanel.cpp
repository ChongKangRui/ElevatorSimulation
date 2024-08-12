// Fill out your copyright notice in the Description page of Project Settings.


#include "ElevatorControlPanel.h"
#include "Elevator.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "W_ElevatorControlPanel.h"
#include "Kismet/GameplayStatics.h"
#include "ElevatorSimulationCharacter.h"

// Sets default values
AElevatorControlPanel::AElevatorControlPanel()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	BoxDetection = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxDetectionComponent"));
	BoxDetection->SetupAttachment(GetRootComponent());
	BoxDetection->OnComponentBeginOverlap.AddDynamic(this, &AElevatorControlPanel::OnBoxDetectionOverlapBegin);
	BoxDetection->OnComponentEndOverlap.AddDynamic(this, &AElevatorControlPanel::OnBoxDetectionOverlapEnd);


	static ConstructorHelpers::FClassFinder<UUserWidget> elevatorControlPanelClassFinder(TEXT("/Game/UI/WBP_ElevatorControlPanel"));
	if (elevatorControlPanelClassFinder.Succeeded())
	{
		m_ElevatorControlPanelWidgetClass = elevatorControlPanelClassFinder.Class;
	}

	PromptWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Input Prompt Widget Component"));
	PromptWidgetComponent->SetupAttachment(BoxDetection);
	PromptWidgetComponent->SetVisibility(false);
}

void AElevatorControlPanel::BeginPlay()
{
	Super::BeginPlay();

	if (FloorNumber != -1) {
		TArray<AActor*> elevatorList;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AElevator::StaticClass(), elevatorList);

		for (auto elevatorActor : elevatorList) {
			if (elevatorActor) {
				if (AElevator* elevator = Cast<AElevator>(elevatorActor)) {
					m_ElevatorContainer.AddUnique(elevator);
				}
			}
		}

	}
}

AElevator* AElevatorControlPanel::GetAssignedElevator() const
{
	return m_AssignedElevator;
}

void AElevatorControlPanel::SetAssignedElevator(AElevator* elevator)
{
	if (elevator)
		m_AssignedElevator = elevator;

}

void AElevatorControlPanel::InteractControlPanel(AElevatorSimulationCharacter* Interacter)
{
	if (m_AssignedElevator) {
		if (Interacter) {
			if (m_CurrentElavatorWidget) {
				RemoveElevatorControlWidget();
				Interacter->ToggleShowMouseCursor(false);
			}
			else {
				CreateElevatorControlWidget();
				Interacter->ToggleShowMouseCursor(true);
			}
		}
	}
	else {
		CallElevator();
	}
}

void AElevatorControlPanel::CallElevator()
{
	FTimerManager& tm = GetWorld()->GetTimerManager();
	if (tm.IsTimerActive(m_CallElevatorTimer)) {
		return;
	}

	if (m_ElevatorContainer.Num() <= 0) {
		UE_LOG(LogTemp, Error, TEXT("Elevator Control Panel: No Elevator Reference in container"));
		return;
	}

	bool hasElevatorOnFloor = m_ElevatorContainer.ContainsByPredicate([this](const TObjectPtr<AElevator>& elevator) {
		return elevator->GetDestinationFloor() == FloorNumber;
		});

	if (hasElevatorOnFloor)
		return;

	FTimerDelegate tempDelegate;
	tempDelegate.BindWeakLambda(this, [&]()
		{
			TObjectPtr<AElevator>* elevator = m_ElevatorContainer.FindByPredicate([this](const TObjectPtr<AElevator>& elevator) {
				return !elevator->IsMoving();
				});
			UE_LOG(LogTemp, Warning, TEXT("Elevator Control Panel: finding elevator to call"));
			if (elevator && (*elevator)) {
				(*elevator)->SetFloorDestination(FloorNumber);
				StopCallElevator();
			}
		});
	tm.SetTimer(m_CallElevatorTimer, tempDelegate, 1.0f, true);
}

void AElevatorControlPanel::StopCallElevator()
{
	GetWorld()->GetTimerManager().ClearTimer(m_CallElevatorTimer);
	m_CallElevatorTimer.Invalidate();
}

void AElevatorControlPanel::CreateElevatorControlWidget()
{
	if (!m_CurrentElavatorWidget) {
		m_CurrentElavatorWidget = CreateWidget<UW_ElevatorControlPanel>(GetWorld(), m_ElevatorControlPanelWidgetClass);
		m_CurrentElavatorWidget->SetControlPanel(this);
		m_CurrentElavatorWidget->AddToViewport();
	}
}

void AElevatorControlPanel::RemoveElevatorControlWidget()
{
	if (m_CurrentElavatorWidget) {
		m_CurrentElavatorWidget->RemoveFromParent();
		m_CurrentElavatorWidget = nullptr;
	}
}

void AElevatorControlPanel::OnBoxDetectionOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player")) {
		if (AElevatorSimulationCharacter* playercharacter = Cast<AElevatorSimulationCharacter>(OtherActor)) {
			PromptWidgetComponent->SetVisibility(true);
			playercharacter->SetElavatorControlPanel(this);
		}
	}
}

void AElevatorControlPanel::OnBoxDetectionOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Player")) {
		if (AElevatorSimulationCharacter* playercharacter = Cast<AElevatorSimulationCharacter>(OtherActor)) {

			if (m_AssignedElevator) {
				RemoveElevatorControlWidget();
				playercharacter->ToggleShowMouseCursor(false);
			}

			PromptWidgetComponent->SetVisibility(false);
			playercharacter->ClearElevatorControlPanel();
		}
	}
}



