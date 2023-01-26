// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Meteorite.h"
#include "GameFramework/Actor.h"
#include "UnrealStudies/Enemy.h"
#include "Boss.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBossDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMeteoriteAttackDelegate, TArray<AMeteorite*>, Meteorites);

UCLASS()
class UNREALSTUDIES_API ABoss : public AEnemy
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABoss();

protected:

	FVector StartLocation;
	
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
	virtual void MeteoriteAttack(float Number, float Radius);

	UFUNCTION()
	virtual void OnMeteoriteHit();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable)
	FBossDelegate OnMeteoriteAttackFinished;
	
	UPROPERTY(BlueprintAssignable)
	FMeteoriteAttackDelegate OnMeteoriteAttackLaunched;

	UFUNCTION(BlueprintCallable)
	FVector GetStartLocation() const { return StartLocation; }

private:

	int CurrentMeteoriteNumber;
};
