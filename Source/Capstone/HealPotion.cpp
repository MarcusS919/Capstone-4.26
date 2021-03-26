// Fill out your copyright notice in the Description page of Project Settings.


#include "HealPotion.h"
#include "CapstoneCharacter.h"
#include "UObject/UObjectIterator.h"

// Sets default values
AHealPotion::AHealPotion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	_rootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	RootComponent = _rootComponent;

	healPotMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Heal Pot Mesh"));
	healPotMesh->SetupAttachment(RootComponent);
	
	
	interactionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Interaction Widget"));
	interactionWidget->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AHealPotion::BeginPlay()
{
	Super::BeginPlay();
	interactionWidget->SetVisibility(false);
}

// Called every frame
void AHealPotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHealPotion::InteractWithMe()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Interacted with heal potion")));
	
	//myCharacter = GetWorld()->GetFirstPlayerController();
	//myCharacter->UpdateHealth(-20.0f);
	for (TObjectIterator<ACapstoneCharacter> Itr; Itr; ++Itr)
	{
		if (Itr->IsA(ACapstoneCharacter::StaticClass()))
		{
			ACapstoneCharacter* actorClass = *Itr;
			actorClass->UpdateHealth(20.0f);
			actorClass = nullptr;
		}
	}
	this->Destroy();
	
}

void AHealPotion::ShowInteractionWidget()
{
	interactionWidget->SetVisibility(true);
}

void AHealPotion::HideInteractionWidget()
{
	interactionWidget->SetVisibility(false);
}

