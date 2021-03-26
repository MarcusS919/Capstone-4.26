// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Grid.h"
#include "ConstructorHelpers.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObstacleMesh.generated.h"

UCLASS()
class CAPSTONE_API AObstacleMesh : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObstacleMesh();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(VisibleAnywhere) UStaticMeshComponent* ObstacleMesh;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverLapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 otherBodyIndex);


	UPROPERTY(EditAnywhere) AActor* otherActor = nullptr;
	UPROPERTY(EditAnywhere) AGrid* grid = nullptr;

	void checkNodes();

};
