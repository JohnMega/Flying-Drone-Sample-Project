// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/FlightDroneController.h"
#include "BehaviorTree/BehaviorTree.h"

DEFINE_LOG_CATEGORY_STATIC(LogFlightDroneController, All, All);

void AFlightDroneController::BeginPlay()
{
	Super::BeginPlay();

	UBehaviorTree* BTLoaded = BehaviorTree.LoadSynchronous();
	if (!BTLoaded)
	{
		UE_LOG(LogFlightDroneController, Error, TEXT("%s::%hs: Failed to load behavior tree"), *GetName(), __FUNCTION__);
		return;
	}

	RunBehaviorTree(BTLoaded);
}