// Fill out your copyright notice in the Description page of Project Settings.


#include "WizardNPC.h"
#include "CapstoneCharacter.h"
#include "UObject/UObjectIterator.h"

// Sets default values
AWizardNPC::AWizardNPC()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	_rootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	RootComponent = _rootComponent;


	wizardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wizard Mesh"));
	wizardMesh->SetupAttachment(RootComponent);


	interactionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Interaction Widget"));
	interactionWidget->SetupAttachment(RootComponent);

	dialogueWIdget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Dialogue Widget"));
	
	waitTime = 3.0f;
}

// Called when the game starts or when spawned
void AWizardNPC::BeginPlay()
{
	Super::BeginPlay();
	interactionWidget->SetVisibility(false);
	dialogueWIdget->SetVisibility(false);
}

// Called every frame
void AWizardNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWizardNPC::InteractWithMe()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Interacted with wizard")));
	
	for (TObjectIterator<ACapstoneCharacter> Itr; Itr; ++Itr)
	{
		if (Itr->IsA(ACapstoneCharacter::StaticClass()))
		{
			ACapstoneCharacter* actorClass = *Itr;
			if (actorClass->GetQuestUIStage() == 0.0f) {
				dialogueWIdget->SetVisibility(true);
				GetWorld()->GetTimerManager().SetTimer(waitTimerHandle, this, &AWizardNPC::HideDialogue, waitTime, false);
				actorClass->UpdateQuestUI(1.0f);
			}

			if (actorClass->GetQuestUIStage() == 1.0f) {
				dialogueWIdget->SetVisibility(true);
				GetWorld()->GetTimerManager().SetTimer(waitTimerHandle, this, &AWizardNPC::HideDialogue, waitTime, false);
			}

			if (actorClass->GetQuestUIStage() == 2.0f) {
				dialogueWIdget->SetVisibility(true);
				GetWorld()->GetTimerManager().SetTimer(waitTimerHandle, this, &AWizardNPC::HideDialogue, waitTime, false);
				actorClass->UpdateQuestUI(1.0f);
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("changed to quest 4")));
			}
			actorClass = nullptr;
		}
		
	}
	
}

void AWizardNPC::ShowInteractionWidget()
{
	interactionWidget->SetVisibility(true);
}

void AWizardNPC::HideInteractionWidget()
{
	interactionWidget->SetVisibility(false);
}

void AWizardNPC::HideDialogue()
{
	dialogueWIdget->SetVisibility(false);
}

