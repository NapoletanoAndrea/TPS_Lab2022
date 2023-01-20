// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss.h"

#include "CollisionDebugDrawingPublic.h"
#include "NavigationSystem.h"
#include "UnrealStudies/TP_ThirdPerson/TP_ThirdPersonCharacter.h"


// Sets default values
ABoss::ABoss()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ChargedAttackSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Charged Attack Spawn Point"));
	ChargedAttackSpawnPoint->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABoss::BeginPlay()
{
	Super::BeginPlay();

	ChargedAttack(0, 200, 50);
}

FVector ABoss::GetBossDestination(float Radius)
{
	FVector MyLocation = GetActorLocation();
	FVector PlayerLocation = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
	PlayerLocation.Z = MyLocation.Z;
	FVector Direction = MyLocation - PlayerLocation;
	Direction.Normalize();
	return PlayerLocation + Direction * Radius;
}

void ABoss::ChargedAttack(float Damage, float Range, float Radius)
{
	FCollisionQueryParams Params;
	AActor* Myself = Cast<AActor>(this);
	Params.AddIgnoredActor(Myself);

	FCollisionShape CollShape = FCollisionShape::MakeSphere(Radius);

	FVector Start = ChargedAttackSpawnPoint->GetComponentLocation();
	FVector End = Start + GetActorForwardVector() * Range;

	TArray<FHitResult> Hits;

	DrawSphereSweeps(GetWorld(), Start, End, Radius, Hits, 5);

	FHitResult Hit;
	bool bHit = GetWorld()->SweepSingleByChannel(Hit, Start, End, FQuat::Identity, ECC_Pawn, CollShape, Params);

	if (bHit)
	{
		ATP_ThirdPersonCharacter* HitPlayer = Cast<ATP_ThirdPersonCharacter>(Hit.Actor.Get());

		if (HitPlayer)
		{
			HitPlayer->GetHealthComponent()->GetDamage(Damage);
		}
	}
}

// Called every frame
void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
