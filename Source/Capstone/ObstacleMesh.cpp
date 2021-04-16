// Fill out your copyright notice in the Description page of Project Settings.


#include "ObstacleMesh.h"
#include "AttackActor.h"
#include "Engine/Engine.h"
#include "Components/BoxComponent.h"

// Sets default values
AObstacleMesh::AObstacleMesh()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ObstacleMesh = CreateDefaultSubobject<UStaticMeshComponent>("obstacle Mesh");
}

// Called when the game starts or when spawned
void AObstacleMesh::BeginPlay()
{
	
	Super::BeginPlay();
	
	UBoxComponent* Component = Cast<UBoxComponent>(GetComponentByClass(UBoxComponent::StaticClass()));

	if (Component) {
		
		
		Component->OnComponentBeginOverlap.AddDynamic(this, &AObstacleMesh::OnOverlapBegin);
		Component->OnComponentEndOverlap.AddDynamic(this, &AObstacleMesh::OnOverLapEnd);
	

	}
	
}

// Called every frame
void AObstacleMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UBoxComponent* Component = Cast<UBoxComponent>(GetComponentByClass(UBoxComponent::StaticClass()));
	if (Component) {
	

		Component->OnComponentBeginOverlap.AddDynamic(this, &AObstacleMesh::OnOverlapBegin);
		Component->OnComponentEndOverlap.AddDynamic(this, &AObstacleMesh::OnOverLapEnd);


	}
}

void AObstacleMesh::OnOverlapBegin(
	class UPrimitiveComponent* OverlappedComp,
	class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	
	
	if (OtherActor && (OtherActor != this)) {
		FVector hitPosition = OtherActor->GetActorLocation();
		AGrid* grid_ = dynamic_cast<AGrid*>(OtherActor);
		grid_->checkWall = true;

		


	}

}

void AObstacleMesh::OnOverLapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 otherBodyIndex){

	if (OtherActor && (OtherActor != this)) {
		
	}
}





