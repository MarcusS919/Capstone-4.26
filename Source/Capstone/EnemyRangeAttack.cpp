// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyRangeAttack.h"
#include "ObstacleMesh.h"
#include "CapstoneCharacter.h"
#include "PathFinding.h"
#include "Engine/Engine.h"


// Sets default values
AEnemyRangeAttack::AEnemyRangeAttack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("Projectile Movement"));
}

// Called when the game starts or when spawned
void AEnemyRangeAttack::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &AEnemyRangeAttack::OnOverlap);
}

// Called every frame
void AEnemyRangeAttack::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);
	
	

	if (t == 0) {
		Destroy();

	}
	else {
		t = t - 1;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("me: %f"), t));
	}

	
	
}

void AEnemyRangeAttack::OnOverlap(AActor* MyOverlappedActor, AActor* OtherActor)
{
	
	if (OtherActor != nullptr && OtherActor != this)
	{
		
		class ACapstoneCharacter* MyCharacter = Cast<ACapstoneCharacter>(OtherActor);
		class 	AObstacleMesh* Wall = Cast<AObstacleMesh>(OtherActor);
		if (MyCharacter)
		{
			MyCharacter->UpdateHealth(-10.0f);
			Destroy();
		}

		if (Wall) {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT(" Ai hit wall %f"), t));
			Destroy();
		}


	}
}



void AEnemyRangeAttack::ShootProjectile(float Speed)
{
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->SetVelocityInLocalSpace(FVector::ForwardVector * Speed);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("shooting: %f"), Speed));
}

void AEnemyRangeAttack::DestoryObj()
{
	Destroy();
}
