
// Fill out your copyright notice in the Description page of Project Settings.

#include "AttackActor.h"
#include "ObstacleMesh.h"
#include "PathFinding.h"
#include "Engine/Engine.h"


// Sets default values
AAttackActor::AAttackActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("Projectile Movement"));

}

// Called when the game starts or when spawned
void AAttackActor::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &AAttackActor::OnOverlap);

}

// Called every frame
void AAttackActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

	//ActorLocation = GetActorLocation();
	//ActorLocation.X += 10.0f;
	//SetActorLocation(ActorLocation, true);
	if (t == 0) {
		Destroy();
		
	}
	else {
		t = t - 1;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("me: %f"), t));
	}

}

void AAttackActor::OnOverlap(AActor* MyOverlappedActor, AActor* OtherActor)
{
	if (OtherActor != nullptr && OtherActor != this)
	{
		
		
		class APathFinding* MyCharacter = Cast<APathFinding>(OtherActor);
		class 	AObstacleMesh* Wall = Cast<AObstacleMesh>(OtherActor);

	
		if (MyCharacter)
		{
			MyCharacter->UpdateHealth(-50.0f);
			Destroy();
		}
		if (Wall) {
			
			Destroy();
		}
	}
}

void AAttackActor::ShootProjectile(float Speed)
{
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->SetVelocityInLocalSpace(FVector::ForwardVector * Speed);
	
}


