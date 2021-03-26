// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "PathFindingNode.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid.generated.h"

UCLASS()
class CAPSTONE_API AGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrid();
	~AGrid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere) AActor* BottomLeft = nullptr;
	UPROPERTY(EditAnywhere) AActor* TopRight = nullptr;

	
		
	
	UPROPERTY(VisibleAnywhere, Category = "Trigger Box") class UBoxComponent* GridTrigger;

	PathFindingNode*** MyGrid;

	int nodeWidth;
	int nodeHeight;

	bool checkWall;
	UFUNCTION()
		void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	PathFindingNode* WorldToNode(FVector Position);
	TArray<PathFindingNode*> GetNeighbours(PathFindingNode* current);

private:

	float StartY, StartX;
	float EndY, EndX;

	int YNodeCount;
	int XNodeCount;

	void CreateGrid();
	void SetGrid();

	void checkObstacle(FVector worldPosition);

};
