// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatAudio.h"

// Sets default values
ACombatAudio::ACombatAudio()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACombatAudio::BeginPlay()
{
	Super::BeginPlay();

	AActor* CombatManagerActor = UGameplayStatics::GetActorOfClass(GetWorld(), ACombatManager::StaticClass());
	ACombatManager* CombatManager = Cast<ACombatManager>(CombatManagerActor);
	CombatManager->OnStatusUpdated.AddDynamic(this, &ACombatAudio::UpdateSound);

	AudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), CalmSound, 0, 1, 0);
	UpdateSound(CombatManager->CombatStatus, true);
}

void ACombatAudio::UpdateSound(TEnumAsByte<ECombatStatus> CombatStatus, bool bHasChanged)
{
	USoundCue* SoundCue = CalmSound;
	if(!bHasChanged)
	{
		return;
	}
	
	switch (CombatStatus)
	{
	case Calm:
		SoundCue = CalmSound;
		break;
	case Stealth:
		SoundCue = StealthSound;
		break;
	case Fighting:
		SoundCue = FightingSound;
		break;
	default: ;
	}
	
	AudioComponent->SetVolumeMultiplier(1);
	AudioComponent->SetSound(SoundCue);
	AudioComponent->Play();
}


// Called every frame
void ACombatAudio::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

