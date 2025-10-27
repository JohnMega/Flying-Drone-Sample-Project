// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FlightDroneController.generated.h"

class UBehaviorTree;

UCLASS()
class TESTPROJECT_API AFlightDroneController : public AAIController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Behavior")
	TSoftObjectPtr<UBehaviorTree> BehaviorTree;
};
