// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_ElevatorControlPanel.generated.h"

class AElevatorControlPanel;
class AElevatorSimulationCharacter;

UCLASS()
class ELEVATORSIMULATION_API UW_ElevatorControlPanel : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/*Used to set the reference only, functionality will be done in blueprint*/
	void SetControlPanel(AElevatorControlPanel* panel, AElevatorSimulationCharacter* character);

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AElevatorControlPanel> ControlPanel;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AElevatorSimulationCharacter> PossessCharacter;
};
