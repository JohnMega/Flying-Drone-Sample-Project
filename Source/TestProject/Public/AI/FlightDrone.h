// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "FlightDrone.generated.h"

class USkeletalMeshComponent;
class USphereComponent;
class UHealthComponent;

UCLASS()
class TESTPROJECT_API AFlightDrone : public APawn, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AFlightDrone();

	// Begin APawn overrides
	virtual void Tick(float DeltaTime) override;
	// End APawn overrides

	// Begin APawn overrides
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
	// End APawn overrides

protected:
	// Begin APawn overrides
	virtual void BeginPlay() override;
	// End APawn overrides

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UHealthComponent> HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|RotateToPlayer")
	float RTPInterpSpeed = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|VelocityRotation")
	float VRInterpSpeed = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|VelocityRotation")
	float VRProjectOnForwardMaxAngle = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|VelocityRotation")
	float VRProjectOnLeftMaxAngle = 45.0f;


private:
	void RotateToPlayer(float DeltaTime);
	void SetVelocityRotation(float DeltaTime);
	float ProjectVelocityOnVector(FVector TargetVector, float MaxAngle);

private:
	TWeakObjectPtr<APawn> CachedPlayerPawn = nullptr;
};
