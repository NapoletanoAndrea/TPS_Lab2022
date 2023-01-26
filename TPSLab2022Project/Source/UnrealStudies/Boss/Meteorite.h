// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Meteorite.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMeteoriteDelegate);

UCLASS()
class UNREALSTUDIES_API AMeteorite : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMeteorite();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable)
	FMeteoriteDelegate OnMeteoriteSpawned;

	UPROPERTY(BlueprintAssignable)
	FMeteoriteDelegate OnMeteoriteHit;
};
