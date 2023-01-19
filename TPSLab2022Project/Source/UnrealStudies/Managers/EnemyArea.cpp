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

void AEnemyArea::AddEnemy(AEnemy* Enemy)
{
	if(IsValid(Enemy) && !Enemies.Contains(Enemy) && !Enemy->IsDead())
	{
		Enemies.Add(Enemy);
		Enemy->OnDeath.AddDynamic(this, &AEnemyArea::RemoveEnemy);
		OnAreaUpdated.Broadcast(this);
	}
}

void AEnemyArea::RemoveEnemy(AEnemy* Enemy)
{
	if(IsValid(Enemy) && Enemies.Contains(Enemy))
	{
		Enemies.Remove(Enemy);
		Enemy->OnDeath.RemoveDynamic(this, &AEnemyArea::RemoveEnemy);
		OnAreaUpdated.Broadcast(this);
	}
}

void AEnemyArea::FillEnemyArray(TArray<AActor*> EnemyActors)
{
	for (const auto &EnemyActor : EnemyActors)
	{
		AddEnemy(Cast<AEnemy>(EnemyActor));
	}
}

// Called every frame
void AEnemyArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AEnemyArea::ContainsLiveEnemies() const
{
	return Enemies.Num() > 0;
}

void AEnemyArea::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, TEXT("Entered"));
	
	if(OtherActor->IsA(ATP_ThirdPersonCharacter::StaticClass()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, TEXT("Player Entered"));
		bContainsPlayer = true;
		OnAreaUpdated.Broadcast(this);
		return;
	}
	
	AEnemy* OverlappedEnemy = Cast<AEnemy>(OtherActor);
	AddEnemy(OverlappedEnemy);
}

void AEnemyArea::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	
	if(OtherActor->IsA(ATP_ThirdPersonCharacter::StaticClass()))
	{
		bContainsPlayer = false;
		OnAreaUpdated.Broadcast(this);
		return;
	}
	
	RemoveEnemy(Cast<AEnemy>(OtherActor));
}



