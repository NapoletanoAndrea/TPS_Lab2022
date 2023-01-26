// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Boss.h"
#include "GameFramework/Actor.h"
#include "BossAudio.generated.h"

UCLASS()
class UNREALSTUDIES_API ABossAudio : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABossAudio();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere)
	USoundCue* CalmSound;

	UPROPERTY(EditAnywhere)
	USoundCue* FightingSound;

	UFUNCTION()
	void UpdateSound(TEnumAsByte<EBossState> BossState);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
