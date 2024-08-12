// Fill out your copyright notice in the Description page of Project Settings.


#include "Elevator/W_ElevatorControlPanel.h"

void UW_ElevatorControlPanel::SetControlPanel(AElevatorControlPanel* panel, AElevatorSimulationCharacter* character)
{
	ControlPanel = panel;
	PossessCharacter = character;
}
