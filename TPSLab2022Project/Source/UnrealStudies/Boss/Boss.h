// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Meteorite.h"
#include "GameFramework/Actor.h"
#include "UnrealStudies/Enemy.h"
#include "Boss.generated.h"

UENUM()
enum EBossState
{
	Unaggroed
	UMETA(DisplayName = "Unaggroed"),
	
	Calm
	UMETA(DisplayName = "Calm"),

	Fighting
	UMETA(DisplayName = "Fighting")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBossDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMeteoriteAttackDelegate, TArray<AMeteorite*>, Meteorites);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBossStateDelegate, TEnumAsByte<EBossState>, BossState);

UCLASS()
class UNREALSTUDIES_API ABoss : public AEnemy
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABoss();

protected:

	UPROPERTY(BlueprintReadOnly)
	FVector StartLocation;

	UPROPERTY(BlueprintReadOnly)
	FRotator StartRotation;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* ChargedAttackSpawnPoint;

	UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true))
	FVector MeteoriteSpawnLocation;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AMeteorite> MeteoriteClass;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	FVector GetBossDestination(float Radius);
	
	UFUNCTION(BlueprintCallable)
	virtual void ChargedAttack(float Damage, float Range, float Radius);

	UFUNCTION(BlueprintCallable)
	virtual void MeteoriteAttack(float Number, float Radius, float Distance, float Damage);

	UFUNCTION()
	virtual void OnMeteoriteHit();

	UFUNCTION(BlueprintCallable)
	void SetBossState(TEnumAsByte<EBossState> NewState);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TEnumAsByte<EBossState> BossState;

	UPROPERTY(BlueprintAssignable)
	FBossDelegate OnChargedAttackExecuted;

	UPROPERTY(BlueprintAssignable)
	FBossDelegate OnMeteoriteAttackFinished;
	
	UPROPERTY(BlueprintAssignable)
	FMeteoriteAttackDelegate OnMeteoriteAttackLaunched;
	
	UPROPERTY(BlueprintAssignable)
	FBossStateDelegate OnBossStateChanged;

	UFUNCTION(BlueprintCallable)
	FVector GetStartLocation() const { return StartLocation; }

private:

	int CurrentMeteoriteNumber;
};
