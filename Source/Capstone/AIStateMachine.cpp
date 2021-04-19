// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStateMachine.h"
#include "Engine/Engine.h"
// Sets default values
AAIStateMachine::AAIStateMachine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAIStateMachine::BeginPlay()
{
	Super::BeginPlay();
	currentState = PATROL;
}

// Called every frame
void AAIStateMachine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (Ai->health <= 0) {
		Destroy();
		
	}

	AIPos = Ai->GetActorLocation();
	PlayerPos = Player->GetActorLocation();
	distance = (AIPos - PlayerPos).Size();

	//switching between the AI states
	switch (currentState) {
	case PATROL:
		Ai->Patrol();
		//checking if the player has came into case range
		if (distance < 1000) {
			currentState = CHASE;
		}
		break;
	
	case CHASE:
		Ai->Chase();
		//checking to see if the player has either got to fair away or is in attack range or if the player is too close to a range Ai
		if (distance > 1000) {
			currentState = PATROL;
		}
		//for when the range Ai can go to attack state
		if (distance < 600 && distance > 300 && Ai->isRange == true) {
			currentState = ATTACK;
		}
		//for when the melee Ai can go to attack state
	 if (distance < 300 && Ai->isRange == false) {
			Ai->Attack();
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT(" attacking the player %f")));
		}

		if (distance < 300 && Ai->isRange == true) {
			currentState = FLEE;
		}
		break;

	case ATTACK:
		
		Ai->Attack();
		
		//checking if the player has escaped or is too close
		if (distance > 800) {
			currentState = PATROL;
		}
		if (distance < 300) {
			currentState = FLEE;
		}
		
		break;

	case FLEE:
		Ai->flee();
		//when the Ai reaches the safe area its has fleed
		if (Ai->isFleeing == false) {
			currentState = PATROL;
		}

		break;

	}
}

