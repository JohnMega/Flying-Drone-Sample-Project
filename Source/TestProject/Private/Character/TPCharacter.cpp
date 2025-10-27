// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TPCharacter.h"
// Engine
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"

ATPCharacter::ATPCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
}

void ATPCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	CachedPlayerController = Cast<APlayerController>(GetController());
	if (!CachedPlayerController.IsValid())
	{
		return;
	}
	
	if (ULocalPlayer* LocalPlayer = CachedPlayerController->GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			InputSystem->AddMappingContext(DefaultIMC.LoadSynchronous(), 0);
		}
	}
}

void ATPCharacter::Server_Test_Implementation()
{
	UE_LOG(LogTemp, Error, TEXT("GGGServer"));
	Multicast_Test();
}

void ATPCharacter::Multicast_Test_Implementation()
{
	UE_LOG(LogTemp, Error, TEXT("GGGClient"));
}

void ATPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//StaticMeshComponent->SetWorldLocation(GetRootComponent()->GetComponentLocation() + FVector{0,0,40});
	//FRotator Rotation = UKismetMathLibrary::FindLookAtRotation({ 0,0,0 }, GetComponentByClass<UCameraComponent>()->GetForwardVector());
	//StaticMeshComponent->SetWorldRotation(UKismetMathLibrary::RInterpTo(StaticMeshComponent->GetComponentRotation(), Rotation, DeltaTime, RotationInterpToSpeed));
}

void ATPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!Input)
		return;

	Input->BindAction(MoveIA.LoadSynchronous(), ETriggerEvent::Triggered, this, &ATPCharacter::MoveIAProcess);
	Input->BindAction(LookIA.LoadSynchronous(), ETriggerEvent::Triggered, this, &ATPCharacter::LookIAProcess);
	Input->BindAction(JumpIA.LoadSynchronous(), ETriggerEvent::Triggered, this, &ATPCharacter::Jump);
}

void ATPCharacter::MoveIAProcess(const FInputActionInstance& Instance)
{
	FVector2D MoveVector = Instance.GetValue().Get<FVector2D>();
	if (MoveVector.X)
	{
		AddMovementInput(GetActorForwardVector() * GetCharacterMovement()->GetMaxSpeed(), MoveVector.X);
	}
	if (MoveVector.Y)
	{
		AddMovementInput(GetActorRightVector() * GetCharacterMovement()->GetMaxSpeed(), MoveVector.Y);
	}
}

void ATPCharacter::LookIAProcess(const FInputActionInstance& Instance)
{
	FVector2D LookVector = Instance.GetValue().Get<FVector2D>();
	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(LookVector.Y);
}