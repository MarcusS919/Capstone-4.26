// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "LevelChanger.generated.h"

UCLASS()
class CAPSTONE_API ALevelChanger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelChanger();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Changer Info")
		FString levelString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Changer Info")
		float questCompleteStage;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnOverlap(AActor* MyOverlappedActor, AActor* OtherActor);

};
