// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DroneWeapon.generated.h"

UCLASS()
class TESTPROJECT_API ADroneWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ADroneWeapon();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
	TSoftClassPtr<AActor> WeaponProjectileClass;

	// Reload time after shots are fired
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
	float WeaponReloadTime = 1.0f;

	// Waiting time between shots
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
	float WeaponQueueTime = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings", meta = (ClampMin = "1"))
	int32 WeaponProjectilesNum = 3;

private:
	void WeaponReloadTimerProcess();
	void WeaponQueueTimerProcess();
	void SpawnProjectile();

private:
	TWeakObjectPtr<APawn> CachedPlayerPawn = nullptr;
	FTimerHandle WeaponReloadTimer;
	FTimerHandle WeaponQueueTimer;
	int32 WeaponReleasedProjectilesNum = 0;
};
