// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnrealStudies/Enemy.h"
#include "Boss.generated.h"

UCLASS()
class UNREALSTUDIES_API ABoss : public AEnemy
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABoss();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* ChargedAttackSpawnPoint;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	FVector GetBossDestination(float Radius);
	
	UFUNCTION(BlueprintCallable)
	virtual void ChargedAttack(float Damage, float Range, float Radius);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
