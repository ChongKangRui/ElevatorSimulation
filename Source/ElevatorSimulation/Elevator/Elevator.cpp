// Fill out your copyright notice in the Description page of Project Settings.


#include "Elevator.h"
#include "Components/SplineComponent.h"

AElevator::AElevator()
{
	PrimaryActorTick.bCanEverTick = false;

	DefaultsSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultsRoot"));
	DefaultsSceneRoot->SetupAttachment(RootComponent);
	
	ElevatorRoot = CreateDefaultSubobject<USceneComponent>(TEXT("ElevatorRoot"));
	ElevatorRoot->SetupAttachment(DefaultsSceneRoot);
	
	ElevatorSpline = CreateDefaultSubobject<USplineComponent>(TEXT("ElevatorSpline"));
	ElevatorSpline->SetupAttachment(DefaultsSceneRoot);

	for (int i = 0; i < ElevatorSpline->GetNumberOfSplinePoints(); i++) {
		ElevatorSpline->SetSplinePointType(i,ESplinePointType::Constant);
	}

	bReplicates = true;
	SetReplicateMovement(true);
	
}

void AElevator::BeginPlay()
{
	Super::BeginPlay();

	FVector pointLocation = ElevatorSpline->GetWorldLocationAtSplinePoint(DefaultsStartFloor);
	ElevatorRoot->SetWorldLocation(pointLocation);
	m_CurrentFloor = DefaultsStartFloor;
}

void AElevator::SetFloorDestination(const int floorToReach)
{
	if (floorToReach >= ElevatorSpline->GetNumberOfSplinePoints()) {
		UE_LOG(LogTemp, Error, TEXT("Elevator %s: Invalid Floor"), *GetName());
		return;
	}
	if (IsMoving())
		return;

	if (floorToReach == m_CurrentFloor)
		return;

	UE_LOG(LogTemp, Warning, TEXT("Elevator %s: Start Moving to floor %i"), *GetName(), floorToReach);
	
	m_IsMoving = true;
	m_CurrentFloor = floorToReach;

	/*Start looping for moving interpolation*/
	FTimerDelegate tempDelegate;
	tempDelegate.BindWeakLambda(this, [&]()
		{
			StartMoving(m_CurrentFloor);
		});
	GetWorld()->GetTimerManager().SetTimer(m_MovingTimer, tempDelegate, 0.01f, true);
}

void AElevator::StartMoving(int FloorNumber)
{
	FVector pointLocation = ElevatorSpline->GetWorldLocationAtSplinePoint(FloorNumber);
	float zHeight = FMath::FInterpTo(ElevatorRoot->GetComponentLocation().Z, pointLocation.Z, 0.01f, ElevatorSpeed);

	FVector elevatorLocation = ElevatorRoot->GetComponentLocation();
	ElevatorRoot->SetWorldLocation(FVector(elevatorLocation.X, elevatorLocation.Y, zHeight));

	if (FMath::IsNearlyEqual(pointLocation.Z, elevatorLocation.Z, 1.0f)) {
		StopMoving();
	}
}

void AElevator::StopMoving()
{
	UE_LOG(LogTemp, Warning, TEXT("Elevator %s: Stop Moving"), *GetName());

	m_IsMoving = false;
	GetWorld()->GetTimerManager().ClearTimer(m_MovingTimer);
	m_MovingTimer.Invalidate();
}

int AElevator::GetDestinationFloor() const
{
	return m_CurrentFloor;
}

bool AElevator::IsMoving() const
{
	return m_IsMoving;
}


