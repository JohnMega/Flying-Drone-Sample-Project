// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class UHealthAttributeSet;
class UAbilitySystemComponent;
struct FOnAttributeChangeData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHealthComponentNoParamsSignature);

UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTPROJECT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UHealthComponent();

	UFUNCTION(BlueprintPure)
	float GetHealth() const;

	UFUNCTION(BlueprintPure)
	float GetMaxHealth() const;

public:
	UPROPERTY(BlueprintAssignable)
	FHealthComponentNoParamsSignature OnOwnerDeathDelegate;

	UPROPERTY(BlueprintAssignable)
	FHealthComponentNoParamsSignature OnOwnerReceivedDamageDelegate;

	UPROPERTY(BlueprintAssignable)
	FHealthComponentNoParamsSignature OnOwnerHealthChangedDelegate;

protected:

	// Begin UActorComponent overrides
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// End UActorComponent overrides

	void InitializeHealthAttributes();

	UFUNCTION(BlueprintCallable)
	void UnbindDelegates();
	void BindDelegates();

	UFUNCTION(BlueprintImplementableEvent)
	void HandleOwnerReceiveDamage();

	UFUNCTION(BlueprintImplementableEvent)
	void HandleOwnerDeath();

	UFUNCTION(BlueprintImplementableEvent)
	void HandleOwnerHealthChanged();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	float StartMaxHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	float StartCurrentHealth = 100.0f;

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UAbilitySystemComponent> CachedOwnerASC = nullptr;

	UPROPERTY()
	TObjectPtr<UHealthAttributeSet> HealthAttributeSet = nullptr;

private:
	void OnHealthAttributeChanged(const FOnAttributeChangeData& Data);
};
