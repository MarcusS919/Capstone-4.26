// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionInterface.h"
#include "CapstoneCharacter.h"
#include "Components/WidgetComponent.h"
#include "HealPotion.generated.h"

UCLASS()
class CAPSTONE_API AHealPotion : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHealPotion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void InteractWithMe() override;
	virtual void ShowInteractionWidget() override;
	virtual void HideInteractionWidget() override;

private:
	UPROPERTY(EditAnywhere)
		USceneComponent* _rootComponent;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* healPotMesh;

	UPROPERTY(EditAnywhere)
		class UWidgetComponent* interactionWidget;

};
