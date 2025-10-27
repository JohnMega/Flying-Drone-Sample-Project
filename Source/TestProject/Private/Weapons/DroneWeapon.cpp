// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/DroneWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogDroneWeapon, All, All);

ADroneWeapon::ADroneWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADroneWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	CachedPlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!CachedPlayerPawn.IsValid())
	{
		UE_LOG(LogDroneWeapon, Error, TEXT("%s::%hs: Failed to get player pawn"), *GetName(), __FUNCTION__);
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(WeaponReloadTimer, this, &ADroneWeapon::WeaponReloadTimerProcess, WeaponReloadTime, false);
}

void ADroneWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CachedPlayerPawn.IsValid())
	{
		return;
	}

	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CachedPlayerPawn->GetActorLocation()));
}

void ADroneWeapon::WeaponReloadTimerProcess()
{
	GetWorld()->GetTimerManager().SetTimer(WeaponQueueTimer, this, &ADroneWeapon::WeaponQueueTimerProcess, WeaponQueueTime, false);
}

void ADroneWeapon::WeaponQueueTimerProcess()
{
	SpawnProjectile();
	if (++WeaponReleasedProjectilesNum >= WeaponProjectilesNum)
	{
		WeaponReleasedProjectilesNum = 0;
		GetWorld()->GetTimerManager().SetTimer(WeaponReloadTimer, this, &ADroneWeapon::WeaponReloadTimerProcess, WeaponReloadTime, false);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(WeaponQueueTimer, this, &ADroneWeapon::WeaponQueueTimerProcess, WeaponQueueTime, false);
	}
}

void ADroneWeapon::SpawnProjectile()
{
	UClass* WPCLoaded = WeaponProjectileClass.LoadSynchronous();
	if (!WPCLoaded)
	{
		UE_LOG(LogDroneWeapon, Error, TEXT("%s::%hs: Failed to load WeaponProjectileClass"), *GetName(), __FUNCTION__);
		return;
	}

	FActorSpawnParameters ProjectileSpawnParams;
	ProjectileSpawnParams.Owner = this;
	GetWorld()->SpawnActor<AActor>(WPCLoaded, GetActorTransform(), ProjectileSpawnParams);
}