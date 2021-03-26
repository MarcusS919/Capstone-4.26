// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include "DrawDebugHelpers.h"

#include "Components/BoxComponent.h"
#include "Engine/Engine.h"

// Sets default values
AGrid::AGrid(){
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GridTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Grid iteration trigger"));
	GridTrigger->SetNotifyRigidBodyCollision(true);
	GridTrigger->BodyInstance.SetCollisionProfileName("BlockAllDynamic");
	GridTrigger->OnComponentHit.AddDynamic(this, &AGrid::OnCompHit);

}

AGrid::~AGrid(){

	//for (int x = 0; x < XNodeCount; x++) {
	//	for (int y = 0; y < YNodeCount; y++) {
	//	//	UE_LOG(LogTemp, Warning, TEXT("[Delete Node Data] : %d, %d"), x, y);
	//		delete[] MyGrid[x][y];
	//	}
	//	delete[] MyGrid[x];
	//}
	//delete[] MyGrid;
	//UE_LOG(LogTemp, Warning, TEXT("[Delete Node Data Success]"));
}

// Called when the game starts or when spawned
void AGrid::BeginPlay(){
	Super::BeginPlay();
	if (BottomLeft && TopRight) {
		CreateGrid();
	}
	
}

// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



PathFindingNode* AGrid::WorldToNode(FVector Position){
	
	int nodeY = FMath::RoundToInt((Position.Y - StartY) / nodeWidth);
	int nodeX = FMath::RoundToInt((Position.X - StartX) / nodeHeight);
	return MyGrid[nodeX][nodeY];
}

TArray<PathFindingNode*> AGrid::GetNeighbours(PathFindingNode* current){
	
	TArray<PathFindingNode*> neighbourNodes;

	if (current->arrayY > 0) {

		neighbourNodes.Add(MyGrid[current->arrayX][current->arrayY - 1]);
	}
	if (current->arrayY < YNodeCount - 1) {
		neighbourNodes.Add(MyGrid[current->arrayX][current->arrayY + 1]);
	}
	if (current->arrayX > 0) {
		neighbourNodes.Add(MyGrid[current->arrayX -1][current->arrayY]);
	}
	if (current->arrayX < XNodeCount - 1) {
		neighbourNodes.Add(MyGrid[current->arrayX + 1][current->arrayY]);
	}
	return neighbourNodes;
}

void AGrid::CreateGrid(){
	nodeHeight = 100;
	nodeWidth = 100;

	StartX = BottomLeft->GetActorLocation().X;
	StartY = BottomLeft->GetActorLocation().Y;
	EndX = TopRight->GetActorLocation().X;
	EndY = TopRight->GetActorLocation().Y;

//UE_LOG(LogTemp, Warning, TEXT("[Start Position] : %f, %f"), StartX, StartY);
	//UE_LOG(LogTemp, Warning, TEXT("[End Position] : %f, %f"), EndX, EndY);

	XNodeCount = FMath::RoundToInt(((EndX - StartX) / nodeHeight)) + 1;
	YNodeCount = FMath::RoundToInt(((EndY - StartY) / nodeWidth)) + 1;

	//UE_LOG(LogTemp, Warning, TEXT("[XNodeCount, YNodeCount] : %d, %d"), XNodeCount, YNodeCount);

	MyGrid = new PathFindingNode**[XNodeCount];
	for (int i = 0; i < XNodeCount; i++){
		MyGrid[i] = new PathFindingNode * [YNodeCount];

	}

	SetGrid();
	
}

void AGrid::SetGrid(){
	for (int x = 0; x < XNodeCount; x++) {
		
		for (int y = 0; y < YNodeCount; y++) {
			
			float worldY = StartY + (nodeWidth * y);
			float worldX = StartX + (nodeHeight * x);
			float worldZ = GetActorLocation().Z;
			float nodeSize = nodeHeight + nodeWidth;
			FVector worldPosition = FVector(worldX, worldY, worldZ);

			checkObstacle(worldPosition);
			

			MyGrid[x][y] = new PathFindingNode(x, y, worldPosition, checkWall);
			
			//DrawDebugBox(GetWorld(), worldPosition, FVector(worldX, worldY , worldZ), FColor::Green, true, -1, 0, 10);
		}
	}
}

void AGrid::checkObstacle(FVector worldPosition){
	
	checkWall = false;
	GridTrigger->SetRelativeLocation(worldPosition);
	GridTrigger->SetRelativeLocation(FVector(StartX, StartY, 0.f));
}

void AGrid::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL)) {
		if (GEngine) {

			//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("I Hit: %s"), *OtherActor->GetName()));
		}
	}
}