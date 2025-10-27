// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/FlightDrone.h"
// Engine
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
// Local
#include "Components/HealthComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogFlightDrone, All, All);

AFlightDrone::AFlightDrone()
{
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	SetRootComponent(SkeletalMesh);

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetupAttachment(SkeletalMesh);

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
}

void AFlightDrone::BeginPlay()
{
	Super::BeginPlay();
	
	CachedPlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!CachedPlayerPawn.IsValid())
	{
		UE_LOG(LogFlightDrone, Error, TEXT("%s::%hs: Failed to get player pawn"), *GetName(), __FUNCTION__);
		return;
	}

	AbilitySystemComponent->InitAbilityActorInfo(GetController() ? GetController() : (AActor*)this, this);
}

void AFlightDrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!UKismetMathLibrary::Vector_IsNearlyZero(GetVelocity(), 0.001f))
	{
		RotateToPlayer(DeltaTime);
		SetVelocityRotation(DeltaTime);
	}
}

void AFlightDrone::RotateToPlayer(float DeltaTime)
{
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CachedPlayerPawn->GetActorLocation());
	SetActorRotation(UKismetMathLibrary::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, RTPInterpSpeed));
}

void AFlightDrone::SetVelocityRotation(float DeltaTime)
{
	FRotator TargetRotation(ProjectVelocityOnVector(GetActorForwardVector(), VRProjectOnForwardMaxAngle), GetActorRotation().Yaw, ProjectVelocityOnVector(GetActorRightVector() * -1.0f, VRProjectOnLeftMaxAngle));
	SetActorRotation(UKismetMathLibrary::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, VRInterpSpeed));
}

float AFlightDrone::ProjectVelocityOnVector(FVector TargetVector, float MaxAngle)
{
	FVector ProjectedVelocity = UKismetMathLibrary::ProjectVectorOnToVector(GetVelocity(), TargetVector);
	float UnsignedAngle = FMath::GetMappedRangeValueClamped<float>(TRange<float>(0, GetMovementComponent()->GetMaxSpeed()), TRange<float>(0, MaxAngle), ProjectedVelocity.Length());
	return UnsignedAngle * FMath::Sign(FVector::DotProduct(TargetVector, ProjectedVelocity));
}