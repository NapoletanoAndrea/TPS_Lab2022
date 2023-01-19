// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatManager.h"
#include "GameFramework/Actor.h"
#include "CombatAudio.generated.h"

UCLASS()
class UNREALSTUDIES_API ACombatAudio : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACombatAudio();

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void UpdateSound(TEnumAsByte<ECombatStatus> CombatStatus, bool bHasChanged);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	UAudioComponent* AudioComponent;
	
	UPROPERTY(EditAnywhere)
	USoundCue* CalmSound;

	UPROPERTY(EditAnywhere)
	USoundCue* StealthSound;

	UPROPERTY(EditAnywhere)
	USoundCue* FightingSound;

};
