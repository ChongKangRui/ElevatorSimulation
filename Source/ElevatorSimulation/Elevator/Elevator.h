// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Elevator.generated.h"

class USplineComponent;

UCLASS()
class ELEVATORSIMULATION_API AElevator : public AActor
{
	GENERATED_BODY()
	
public:	
	AElevator();

	UFUNCTION(BlueprintCallable)
	void SetFloorDestination(const int floorToReach);

	int GetDestinationFloor() const;

	bool IsMoving() const;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ElevatorSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int DefaultsStartFloor = 0;


protected:
	virtual void BeginPlay() override;
	
	void StartMoving(int FloorNumber);
	void StopMoving();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USceneComponent> DefaultsSceneRoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USceneComponent> ElevatorRoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USplineComponent> ElevatorSpline;

private:
	int m_CurrentFloor;
	bool m_IsMoving;

	FTimerHandle m_MovingTimer;
};
