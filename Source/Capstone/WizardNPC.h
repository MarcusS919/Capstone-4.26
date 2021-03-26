// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionInterface.h"
#include "CapstoneCharacter.h"
#include "DialogueUserWidget.h"
#include "Components/WidgetComponent.h"
#include "WizardNPC.generated.h"

UCLASS()
class CAPSTONE_API AWizardNPC : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWizardNPC();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UWidgetComponent* dialogueWIdget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float waitTime;

	FTimerHandle waitTimerHandle;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void InteractWithMe() override;
	virtual void ShowInteractionWidget() override;
	virtual void HideInteractionWidget() override;

	UFUNCTION()
		void HideDialogue();
private:
	UPROPERTY(EditAnywhere)
		USceneComponent* _rootComponent;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* wizardMesh;

	UPROPERTY(EditAnywhere)
		class UWidgetComponent* interactionWidget;


};
