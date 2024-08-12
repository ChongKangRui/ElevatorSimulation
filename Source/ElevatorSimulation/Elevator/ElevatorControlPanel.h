// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ElevatorControlPanel.generated.h"

class AElevator;
class UW_ElevatorControlPanel;
class UBoxComponent;
class UWidgetComponent;
class AElevatorSimulationCharacter;

UCLASS()
class ELEVATORSIMULATION_API AElevatorControlPanel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AElevatorControlPanel();

	UFUNCTION(BlueprintPure)
	AElevator* GetAssignedElevator() const;

	/*Call this function only when control panel is controlling elevator to each floor instead of calling elevator to specific floor*/
	UFUNCTION(BlueprintCallable)
	void SetAssignedElevator(AElevator* elevator);

	/*Defaults Interaction function from player*/
	void InteractControlPanel(AElevatorSimulationCharacter* Interacter);

public:
	/*-1 will mean that this control panel is controlling the elevator directly. 
	Otheriwse, this control panel should only calling elevator to their respective floor*/
	UPROPERTY(EditInstanceOnly)
	int FloorNumber = -1;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBoxComponent> BoxDetection;

protected:
	UFUNCTION()
	virtual void OnBoxDetectionOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	virtual void OnBoxDetectionOverlapEnd(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	virtual void BeginPlay() override;

	void CallElevator();
	void StopCallElevator();

	void CreateElevatorControlWidget();
	void RemoveElevatorControlWidget();

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UWidgetComponent> PromptWidgetComponent;

private:
	TObjectPtr<AElevator> m_AssignedElevator;
	TArray<TObjectPtr<AElevator>> m_ElevatorContainer;

	FTimerHandle m_CallElevatorTimer;

	TObjectPtr<UW_ElevatorControlPanel> m_CurrentElavatorWidget;
	TSubclassOf<UW_ElevatorControlPanel> m_ElevatorControlPanelWidgetClass;
};
