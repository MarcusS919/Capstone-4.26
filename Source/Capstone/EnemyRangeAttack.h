// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "CapstoneCharacter.h"
#include "EnemyRangeAttack.generated.h"

UCLASS()
class CAPSTONE_API AEnemyRangeAttack : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyRangeAttack();
	FVector ActorLocation;
	UProjectileMovementComponent* ProjectileMovement = nullptr;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere)ACapstoneCharacter* Player;
	UFUNCTION()
		void OnOverlap(AActor* MyOverlappedActor, AActor* OtherActor);
	
	void ShootProjectile(float Speed);
	void DestoryObj();
	float t = 100;
};
