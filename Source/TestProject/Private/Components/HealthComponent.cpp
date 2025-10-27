// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"
// Engine
#include "GameplayEffect.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
// Local
#include "AbilitySystem/AttributeSets/HealthAttributeSet.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All);

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

float UHealthComponent::GetHealth() const
{
	return HealthAttributeSet != nullptr ? HealthAttributeSet->GetHealth() : -1.0f;
}

float UHealthComponent::GetMaxHealth() const
{
	return HealthAttributeSet != nullptr ? HealthAttributeSet->GetMaxHealth() : -1.0f;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	const IAbilitySystemInterface* OwnerASI = Cast<IAbilitySystemInterface>(GetOwner());
	if (!OwnerASI)
	{
		UE_LOG(LogHealthComponent, Error, TEXT("%s::%hs: Failed to cast owner to IAbilitySystemInterface"), *GetOwner()->GetName(), __FUNCTION__);
		return;
	}

	CachedOwnerASC = OwnerASI->GetAbilitySystemComponent();
	if (!CachedOwnerASC.IsValid())
	{
		UE_LOG(LogHealthComponent, Error, TEXT("%s::%hs: Failed to get AbilitySystemComponent from owner"), *GetOwner()->GetName(), __FUNCTION__);
		return;
	}

	InitializeHealthAttributes();
	BindDelegates();
}

void UHealthComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnbindDelegates();

	Super::EndPlay(EndPlayReason);
}

void UHealthComponent::InitializeHealthAttributes()
{
	HealthAttributeSet = const_cast<UHealthAttributeSet*>(CachedOwnerASC->AddSet<UHealthAttributeSet>());
	if (!HealthAttributeSet.Get())
	{
		UE_LOG(LogHealthComponent, Error, TEXT("%s::%hs: Failed to add HealthAttributeSet"), *GetOwner()->GetName(), __FUNCTION__);
		return;
	}

	HealthAttributeSet->SetMaxHealth(StartMaxHealth);
	HealthAttributeSet->SetHealth(StartCurrentHealth);

	CachedOwnerASC->GetGameplayAttributeValueChangeDelegate(HealthAttributeSet->GetHealthAttribute()).AddUObject(this, &UHealthComponent::OnHealthAttributeChanged);
}

void UHealthComponent::BindDelegates()
{
	OnOwnerReceivedDamageDelegate.AddDynamic(this, &UHealthComponent::HandleOwnerReceiveDamage);
	OnOwnerDeathDelegate.AddDynamic(this, &UHealthComponent::HandleOwnerDeath);
	OnOwnerHealthChangedDelegate.AddDynamic(this, &UHealthComponent::HandleOwnerHealthChanged);
}

void UHealthComponent::UnbindDelegates()
{
	OnOwnerReceivedDamageDelegate.RemoveAll(this);
	OnOwnerDeathDelegate.RemoveAll(this);
	OnOwnerHealthChangedDelegate.RemoveAll(this);
}

void UHealthComponent::OnHealthAttributeChanged(const FOnAttributeChangeData& Data)
{
	if (FMath::IsNearlyEqual(Data.OldValue, Data.NewValue) || (Data.NewValue > HealthAttributeSet->GetMaxHealth() || Data.NewValue < 0.0f))
	{
		return;
	}
	
	if (Data.NewValue <= 0.0f)
	{
		OnOwnerDeathDelegate.Broadcast();
	}
	else if (Data.NewValue < Data.OldValue)
	{
		OnOwnerReceivedDamageDelegate.Broadcast();
	}

	OnOwnerHealthChangedDelegate.Broadcast();
}