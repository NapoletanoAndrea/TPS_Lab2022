// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"

#include "Kismet/GameplayStatics.h"
#include "UnrealStudies/HealthComponent.h"

// Sets default values
AEnemyManager::AEnemyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	PrimaryActorTick.TickInterval = 1.0f;

	bFirstCall = true;
}

// Called when the game starts or when spawned
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), AllActors);

	// Enemies.Init(nullptr, AllActors.Num());
	for(const auto &Actor : AllActors)
	{
		Enemies.Add(Cast<AEnemy>(Actor));
	}
}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CombatStatus = UpdateStatus();
	const FString CombatStatusString = UEnum::GetValueAsString(CombatStatus);
	GEngine->AddOnScreenDebugMessage(-1, PrimaryActorTick.TickInterval, FColor::Yellow,
		CombatStatusString);
}

TEnumAsByte<ECombatStatus> AEnemyManager::UpdateStatus()
{
	bool bEnemiesAreDead = true;

	TEnumAsByte<ECombatStatus> NewStatus = Stealth;
	
	if(Enemies.Num() > 0)
	{
		for(const auto &Enemy : Enemies)
		{
			if(!IsValid(Enemy))
			{
				continue;
			}

			if(!IsValid(Enemy->GetController()))
			{
				continue;
			}
			
			const AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(Enemy->GetController());
			const UHealthComponent* HealthComponent = Enemy->GetHealthComponent();

			if(HealthComponent->Health > 0)
			{
				bEnemiesAreDead = false;
			}
		
			if(EnemyAIController->Aggroed)
			{
				NewStatus = Chasing;
				OnStatusUpdated.Broadcast(NewStatus, DidStatusChange(NewStatus));
				return NewStatus;
			}
		}
	}
	
	if(bEnemiesAreDead)
	{
		NewStatus = Calm;
	}

	OnStatusUpdated.Broadcast(NewStatus, DidStatusChange(NewStatus));
	return NewStatus;
}

bool AEnemyManager::DidStatusChange(TEnumAsByte<ECombatStatus> NewStatus)
{
	if(bFirstCall)
	{
		bFirstCall = false;
		return true;
	}

	return CombatStatus != NewStatus;
}

