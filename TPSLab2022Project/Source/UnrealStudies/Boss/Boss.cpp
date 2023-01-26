// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss.h"

#include "CollisionDebugDrawingPublic.h"
#include "UnrealStudies/TP_ThirdPerson/TP_ThirdPersonCharacter.h"


// Sets default values
ABoss::ABoss()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ChargedAttackSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Charged Attack Spawn Point"));
	ChargedAttackSpawnPoint->SetupAttachment(RootComponent);

	BossState = Unaggroed;
}

// Called when the game starts or when spawned
void ABoss::BeginPlay()
{
	Super::BeginPlay();
	
	StartLocation = GetActorLocation();
	StartRotation = GetActorRotation();
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
	Params.AddIgnoredActor(this);

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
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, TEXT("Hit Player"));
			HitPlayer->GetHealthComponent()->GetDamage(Damage);
		}
	}

	OnChargedAttackExecuted.Broadcast();
}

void ABoss::MeteoriteAttack(float Number, float Radius)
{
	TArray<AMeteorite*> Meteorites;
	
	FVector Direction = GetActorForwardVector();
	const float DeltaAngle = 360 / Number;
	CurrentMeteoriteNumber = Number;
	
	for(int i = 0; i < Number; i++)
	{
		FVector Location = GetTransform().TransformPosition(MeteoriteSpawnLocation) + Direction * Radius;
		FRotator Rotation = FRotationMatrix::MakeFromX(Direction).Rotator();
		Meteorites.Add(GetWorld()->SpawnActor<AMeteorite>(MeteoriteClass, Location, Rotation));
		Meteorites[i]->OnMeteoriteHit.AddDynamic(this, &ABoss::OnMeteoriteHit);
		Direction = Direction.RotateAngleAxis(DeltaAngle, FVector::UpVector);
	}

	OnMeteoriteAttackLaunched.Broadcast(Meteorites);
}

void ABoss::OnMeteoriteHit()
{
	CurrentMeteoriteNumber--;
	if(CurrentMeteoriteNumber <= 0)
	{
		OnMeteoriteAttackFinished.Broadcast();
	}
}

void ABoss::SetBossState(TEnumAsByte<EBossState> NewState)
{
	if(NewState != BossState)
	{
		BossState = NewState;
		OnBossStateChanged.Broadcast(BossState);
	}
}


// Called every frame
void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
