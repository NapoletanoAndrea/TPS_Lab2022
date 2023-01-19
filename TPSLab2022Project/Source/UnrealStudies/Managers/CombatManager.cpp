// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatManager.h"
#include "UnrealStudies/EnemyAIController.h"

// Sets default values
ACombatManager::ACombatManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bFirstCall = true;
	CombatStatus = Calm;
}

// Called when the game starts or when spawned
void ACombatManager::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> EnemyActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyAIController::StaticClass(), EnemyActors);

	for (const auto &EnemyActor : EnemyActors)
	{
		AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(EnemyActor);
		EnemyAIController->OnAggro.AddDynamic(this, &ACombatManager::UpdateAggroedEnemies);
		EnemyAIController->OnUnaggro.AddDynamic(this, &ACombatManager::UpdateAggroedEnemies);
	}

	TArray<AActor*> EnemyAreaActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyArea::StaticClass(), EnemyAreaActors);

	for (const auto &EnemyAreaActor : EnemyAreaActors)
	{
		AEnemyArea* EnemyArea = Cast<AEnemyArea>(EnemyAreaActor);
		EnemyArea->OnAreaUpdated.AddDynamic(this, &ACombatManager::UpdateEnemyAreas);
	}
}

// Called every frame
void ACombatManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TEnumAsByte<ECombatStatus> ACombatManager::UpdateStatus()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Red, TEXT("Update Status"));
	TEnumAsByte<ECombatStatus> NewStatus = Calm;

	TEnumAsByte<ECombatStatus> AreaStatus = Stealth;
	for (const auto &EnemyArea : EnemyAreas)
	{
		if(EnemyArea->ContainsLiveEnemies())
		{
			NewStatus = AreaStatus;
			if(EnemyArea->bPermanentFighting)
			{
				AreaStatus = Fighting;
				NewStatus = AreaStatus;
			}
		}
	}

	if(AggroedEnemiesNum > 0)
	{
		NewStatus = Fighting;
		for (const auto &EnemyArea : EnemyAreas)
		{
			EnemyArea->bPermanentFighting = true;
		}
	}

	OnStatusUpdated.Broadcast(NewStatus, DidStatusChange(NewStatus));
	return NewStatus;
}

bool ACombatManager::DidStatusChange(TEnumAsByte<ECombatStatus> NewStatus)
{
	if(bFirstCall)
	{
		bFirstCall = false;
		return true;
	}

	return CombatStatus != NewStatus;
}

void ACombatManager::UpdateAggroedEnemies(AEnemyAIController* EnemyAIController)
{
	if(EnemyAIController->Aggroed)
	{
		EnemyAIControllers.AddUnique(EnemyAIController);
	}
	else if(EnemyAIControllers.Contains(EnemyAIController))
	{
		EnemyAIControllers.Remove(EnemyAIController);
	}
	AggroedEnemiesNum = EnemyAIControllers.Num();
	CombatStatus = UpdateStatus();
}

void ACombatManager::UpdateEnemyAreas(AEnemyArea* EnemyArea)
{
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, TEXT("Update Areas"));
	if(EnemyArea->bContainsPlayer)
	{
		EnemyAreas.AddUnique(EnemyArea);
		CombatStatus = UpdateStatus();
	}
	else
	{
		if(EnemyAreas.Contains(EnemyArea))
		{
			EnemyAreas.Remove(EnemyArea);
			CombatStatus = UpdateStatus();
		}
	}
}


