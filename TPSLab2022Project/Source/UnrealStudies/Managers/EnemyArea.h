// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/BoxComponent.h"
#include "UnrealStudies/Enemy.h"
#include "UnrealStudies/TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "EnemyArea.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnemyAreaDelegate, AEnemyArea*, EnemyArea);

UCLASS()
class UNREALSTUDIES_API AEnemyArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyArea();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere)
	TArray<AEnemy*> Enemies;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void AddEnemy(AEnemy* Enemy);

	UFUNCTION()
	void RemoveEnemy(AEnemy* Enemy);
	
	UFUNCTION(BlueprintCallable)
	void FillEnemyArray(TArray<AActor*> EnemyActors);

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	bool bContainsPlayer;

	UPROPERTY(VisibleAnywhere)
	bool bPermanentFighting;
	
	UFUNCTION(BlueprintCallable)
	bool ContainsLiveEnemies() const;

	UPROPERTY(BlueprintAssignable)
	FEnemyAreaDelegate OnAreaUpdated;
};
