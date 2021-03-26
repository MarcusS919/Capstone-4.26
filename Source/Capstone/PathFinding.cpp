// Fill out your copyright notice in the Description page of Project Settings.


#include "PathFinding.h"
#include "CapstoneCharacter.h"
#include "EnemyRangeAttack.h"
#include "UObject/UObjectIterator.h"
#include "Engine/Engine.h"
// Sets default values
APathFinding::APathFinding()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>("Enemy");
}

// Called when the game starts or when spawned
void APathFinding::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &APathFinding::OnOverlap);
	if (Target != nullptr) {
		//UE_LOG(LogTemp, Log, TEXT("Start Path Finding : %s"), *Target->GetName());
	}
	meleeAttack = false;
	playerInRange = false;
	if (isRange == true) {
		maxHealth = 100.0f;
		health = maxHealth;
		healthPercent = 1.0f;
		projectileSpeed = 1000;
	}
	else {
		maxHealth = 150.0f;
		health = maxHealth;
		healthPercent = 1.0f;
	}
}

// Called every frame
void APathFinding::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);

	if (Target != nullptr) {

		//AStarPathFinding();

		if (FinalNodeList.Num() > 0) {

			//where the Ai wants to go
			destPos = FinalNodeList[0]->Position;

			//where the Ai is at
			FVector currentPos = GetActorLocation();
			destPos.Z = currentPos.Z;

			
			FVector dir = destPos - currentPos;
			//dir.X += 10;
			FQuat dirQuat(dir.Rotation());
			 rot = FQuat::Slerp(GetActorQuat(), dirQuat, 5 * DeltaTime);

			SetActorRotation(rot);

			SetActorLocation(GetActorLocation() + (dir.GetSafeNormal() * speed));

			if (FVector::Distance(GetActorLocation(), destPos) < 0.001f) {
				//AStarPathFinding();
			}
		}
		else {
			// run path finding
		}
	}

	FVector AIPos = GetActorLocation();
	FVector PlayerPos = player->GetActorLocation();
	float distance = (AIPos - PlayerPos).Size();
	//if the melee enemy can attack
	if (meleeAttack == true) {
		if (t == 0 && distance <= 300) {
			//LookAtPlayer();
			Attack();
			meleeAttack = false;
		}
		else {
			t -= 1;
		}
		
	}
	
}

void APathFinding::LookAtPlayer()
{
	destPos = player->GetActorLocation();
	FVector currentPos = GetActorLocation();
	destPos.Z = currentPos.Z;


	FVector dir = destPos - currentPos;
	FQuat dirQuat(dir.Rotation());
	rot = FQuat::Slerp(GetActorQuat(), dirQuat, 5 * 6);

	SetActorRotation(rot);
	
}

void APathFinding::Patrol(){
	speed = 6.0f;
	isPatroling = true;
	AStarPathFinding();
}

void APathFinding::Chase(){
	isPatroling = false;
	AStarPathFinding();
}

void APathFinding::Attack(){
	speed = 0;
	AStarPathFinding();
	
	if (isRange == true && health >= 0) {
		if (t == 0) {
			//LookAtPlayer();
			SetActorRotation(rot);
			auto Projectile = GetWorld()->SpawnActor<AEnemyRangeAttack>(temp, this->GetActorLocation(), this->GetActorRotation());
			
			Projectile->ShootProjectile(projectileSpeed);
			t += 50;
		}
		else {
			t -= 1;
		}
	}
	 if (isRange == false && health >= 0 && playerInRange == true) {
		
			player->UpdateHealth(-15.0f);
			
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT(" attacking the player %f"), t));
	}
	
}

void APathFinding::countDown()
{
	t -= 1;
	if (t == 0) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT(" attacking the player %f")));
		Attack();
	}
}

void APathFinding::flee(){
	isFleeing = true;
	speed = 15.0f;
	AStarPathFinding();
}
//updaating the health when the enemy is hit
void APathFinding::UpdateHealth(float healthChange_)
{
	health = FMath::Clamp(health += healthChange_, 0.0f, maxHealth);
	healthPercent = health / maxHealth;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("health: %f"), healthPercent));
	if (health <= 0.0f){
		for (TObjectIterator<ACapstoneCharacter> Itr; Itr; ++Itr)
		{
			if (Itr->IsA(ACapstoneCharacter::StaticClass()))
			{
				ACapstoneCharacter* actorClass = *Itr;
				actorClass->UpdateKills(1.0f);
				actorClass->UpdateMana(20.0f);
				actorClass = nullptr;
			}
		}
		Destroy();
	}
}

void APathFinding::OnOverlap(AActor* MyOverlappedActor, AActor* OtherActor)
{
	if (OtherActor != nullptr && OtherActor != this)
	{

		class ACapstoneCharacter* player = Cast<ACapstoneCharacter>(OtherActor);
		
		if (player)
		{
			//player->UpdateHealth(-10.0f);
			playerInRange = true;
			t = 100;
			meleeAttack = true;
		}
		if(!player){
			playerInRange = false;
		}

		


	}
}

void APathFinding::AStarPathFinding(){
	
	PathFindingNode* startNode = MyGrid->WorldToNode(GetActorLocation());
	PathFindingNode* goalNode;

	// picking what goal the ai is going to head for depending on the Ai's state
	if (isPatroling == false && isFleeing == false) {
		goalNode = MyGrid->WorldToNode(Target->GetActorLocation());
	}
	else if(isPatroling == true && atPointOne == false) {
		goalNode = MyGrid->WorldToNode(PartolPoint1->GetActorLocation());
		
	}
	else if (isFleeing == true) {
		goalNode = MyGrid->WorldToNode(FleePoint->GetActorLocation());
	}
	else  {
		goalNode = MyGrid->WorldToNode(PartolPoint2->GetActorLocation());
	
	}
	
	openList.Empty();
	closeList.Empty();

	openList.Add(startNode);
	
	while (openList.Num() > 0) {
		
		PathFindingNode* currentNode = openList[0];

		for (int i = 1; i < openList.Num(); i++) {

			if (openList[i]->GetFCost() < currentNode->GetFCost() || openList[i]->GetGCost() == currentNode->GetFCost() && openList[i]->GetHCost() < currentNode->GetHCost()) {
				currentNode = openList[i];
			}
		}
		openList.Remove(currentNode);
		closeList.Add(currentNode);

		//check to see if the goal the Ai is at is the goal
		if (currentNode == goalNode) {
		
			//if ai is at patrol point one
			if (atPointOne == false) {
				if (startNode == MyGrid->WorldToNode(PartolPoint1->GetActorLocation())){
					atPointOne = true;
				}
			}

			//if ai is at patrol point two
			if (atPointOne == true) {
				if (startNode == MyGrid->WorldToNode(PartolPoint2->GetActorLocation())){
					atPointOne = false;
				}
			}

			//if ai has made it away from the player
			if (startNode == MyGrid->WorldToNode(FleePoint->GetActorLocation())) {
				isFleeing = false;
			}




			FinishFinding(startNode, goalNode);

			for (auto& PathFindingNode : FinalNodeList) {
			//	UE_LOG(LogTemp, Warning, TEXT("The Path is [%d, %d]"), PathFindingNode->arrayX, PathFindingNode->arrayY);
			}
			return;
		}

		for (auto& neighbourNode : MyGrid->GetNeighbours(currentNode)) {
			
			if (!neighbourNode->IsObstacle && !closeList.Contains(neighbourNode)) {
				
				int gCost = currentNode->GetGCost() + ManhattanDistance(*currentNode, *neighbourNode);

				if (!openList.Contains(neighbourNode)) {
					neighbourNode->SetGCost(gCost);

					neighbourNode->SetHCost(ManhattanDistance(*neighbourNode, *goalNode));

					neighbourNode->ParentNode = currentNode;

					if (!openList.Contains(neighbourNode)) {
						openList.Add(neighbourNode);
					}
				}
			}
		}
	}

}

void APathFinding::FinishFinding(PathFindingNode* startNode, PathFindingNode* goalNode){

	TArray<PathFindingNode*> tempPath;

	PathFindingNode *indicator = goalNode;

	while (indicator != startNode) {

		tempPath.Add(indicator);

		indicator = indicator->ParentNode;
		
	}

	Algo::Reverse(tempPath);

		FinalNodeList = tempPath;
		
}

int APathFinding::ManhattanDistance(const PathFindingNode& current, const PathFindingNode& neighbour)
{
	int distance = FMath::Abs((current.arrayX - neighbour.arrayX) * MyGrid->nodeHeight) + FMath::Abs((current.arrayY - neighbour.arrayY) * MyGrid->nodeWidth);
	return distance;
}

