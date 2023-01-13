// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnrealStudies/Enemy.h"
#include "UnrealStudies/EnemyAIController.h"
#include "EnemyManager.generated.h"

UENUM()
enum ECombatStatus
{
	Stealth
	UMETA(DisplayName = "Stealth"),
	
	Chasing
	UMETA(DisplayName = "Chasing"),
	
	Calm
	UMETA(DisplayName = "Calm"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStatusUpdatedDelegate, TEnumAsByte<ECombatStatus>, CombatStatus,
	bool, bStatusChanged);

UCLASS()
class UNREALSTUDIES_API AEnemyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyManager();

	TEnumAsByte<ECombatStatus> CombatStatus;
	TArray<AEnemy*> Enemies;

	UPROPERTY(BlueprintAssignable)
	FStatusUpdatedDelegate OnStatusUpdated;

private:
	
	bool bFirstCall;
	
	TEnumAsByte<ECombatStatus> UpdateStatus();
	bool DidStatusChange(TEnumAsByte<ECombatStatus> NewStatus);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
