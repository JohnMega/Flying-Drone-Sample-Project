// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPCharacter.generated.h"

class UStaticMeshComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionInstance;

UCLASS()
class TESTPROJECT_API ATPCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATPCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Inputs")
	TSoftObjectPtr<UInputMappingContext> DefaultIMC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Inputs")
	TSoftObjectPtr<UInputAction> MoveIA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Inputs")
	TSoftObjectPtr<UInputAction> LookIA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Inputs")
	TSoftObjectPtr<UInputAction> JumpIA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Weapon")
	float RotationInterpToSpeed = 1.0f;

public:
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_Test();
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void Multicast_Test();

private:
	void MoveIAProcess(const FInputActionInstance& Instance);
	void LookIAProcess(const FInputActionInstance& Instance);

private:
	TWeakObjectPtr<APlayerController> CachedPlayerController = nullptr;
};
