// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathFinding.h"
#include "AIStateMachine.generated.h"


enum AiState {
	PATROL,
	CHASE,
	ATTACK,
	FLEE,


};

UCLASS()
class CAPSTONE_API AAIStateMachine : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAIStateMachine();

	UPROPERTY(EditAnywhere) APathFinding* Ai = nullptr;
	UPROPERTY(EditAnywhere) AActor* Player = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	AiState currentState = PATROL;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector AIPos;
	FVector PlayerPos;
	float distance;

};
