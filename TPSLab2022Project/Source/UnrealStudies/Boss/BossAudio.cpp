// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAudio.h"

#include "Boss.h"


// Sets default values
ABossAudio::ABossAudio()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABossAudio::BeginPlay()
{
	Super::BeginPlay();

	AActor* BossActor = UGameplayStatics::GetActorOfClass(GetWorld(), ABoss::StaticClass());
	ABoss* Boss = Cast<ABoss>(BossActor);
	Boss->OnBossStateChanged.AddDynamic(this, &ABossAudio::UpdateSound);

	AudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), CalmSound, 0, 1, 0);
}

void ABossAudio::UpdateSound(TEnumAsByte<EBossState> BossState)
{
	USoundCue* SoundCue = CalmSound;
	
	switch (BossState)
	{
	case Unaggroed:
		AudioComponent->Stop();
		return;
	case Calm:
		SoundCue = CalmSound;
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
void ABossAudio::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

