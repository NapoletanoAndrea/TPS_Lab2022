// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyArea.h"

// Sets default values
AEnemyArea::AEnemyArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(FName("Box Collision"));
	BoxCollision->SetupAttachment(RootComponent);

	BoxCollision->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AEnemyArea::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyArea::FillEnemyArray(TArray<AActor*> EnemyActors)
{
	for (const auto &EnemyActor : EnemyActors)
	{
		Enemies.Add(Cast<AEnemy>(EnemyActor));
	}
}

// Called every frame
void AEnemyArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyArea::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	AEnemy* OverlappedEnemy = Cast<AEnemy>(OtherActor);
	if(IsValid(OverlappedEnemy))
	{
		Enemies.AddUnique(OverlappedEnemy);
	}
}

void AEnemyArea::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	if(Enemies.Contains(OtherActor))
	{
		Enemies.Remove(Cast<AEnemy>(OtherActor));
	}
}



