// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyArea.h"
#include "GameFramework/Actor.h"
#include "UnrealStudies/EnemyAIController.h"
#include "CombatManager.generated.h"

UENUM()
enum ECombatStatus
{
	Stealth
	UMETA(DisplayName = "Stealth"),
	
	Fighting
	UMETA(DisplayName = "Fighting"),
	
	Calm
	UMETA(DisplayName = "Calm"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStatusUpdatedDelegate, TEnumAsByte<ECombatStatus>, CombatStatus,
	bool, bStatusChanged);

UCLASS()
class UNREALSTUDIES_API ACombatManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACombatManager();

	UPROPERTY(VisibleAnywhere)
	TEnumAsByte<ECombatStatus> CombatStatus;

	UPROPERTY(BlueprintAssignable)
	FStatusUpdatedDelegate OnStatusUpdated;

private:
	
	bool bFirstCall;

	UPROPERTY(VisibleAnywhere)
	TArray<AEnemyAIController*> EnemyAIControllers;

	UPROPERTY(VisibleAnywhere)
	int AggroedEnemiesNum;

	UPROPERTY(VisibleAnywhere)
	TArray<AEnemyArea*> EnemyAreas;
	
	TEnumAsByte<ECombatStatus> UpdateStatus();
	
	bool DidStatusChange(TEnumAsByte<ECombatStatus> NewStatus);

	UFUNCTION()
	void UpdateAggroedEnemies(AEnemyAIController* EnemyAIController);

	UFUNCTION()
	void UpdateEnemyAreas(AEnemyArea* EnemyArea);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
