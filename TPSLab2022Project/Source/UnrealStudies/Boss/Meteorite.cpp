// Fill out your copyright notice in the Description page of Project Settings.


#include "Meteorite.h"


// Sets default values
AMeteorite::AMeteorite()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMeteorite::BeginPlay()
{
	Super::BeginPlay();
	OnMeteoriteSpawned.Broadcast();
	
}

void AMeteorite::NotifyActorBeginOverlap(AActor* OtherActor)
{
	OnMeteoriteHit.Broadcast();
	Super::NotifyActorBeginOverlap(OtherActor);
}

// Called every frame
void AMeteorite::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

