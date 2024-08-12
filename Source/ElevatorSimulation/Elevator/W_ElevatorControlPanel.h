// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_ElevatorControlPanel.generated.h"

class AElevatorControlPanel;

UCLASS()
class ELEVATORSIMULATION_API UW_ElevatorControlPanel : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetControlPanel(AElevatorControlPanel* panel);

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AElevatorControlPanel> ControlPanel;
};
