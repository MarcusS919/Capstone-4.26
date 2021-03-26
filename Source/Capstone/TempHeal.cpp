// Fill out your copyright notice in the Description page of Project Settings.


#include "TempHeal.h"
#include "CapstoneCharacter.h"
// Sets default values
ATempHeal::ATempHeal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATempHeal::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &ATempHeal::OnOverlap);
}

// Called every frame
void ATempHeal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ATempHeal::OnOverlap(AActor* MyOverlappedActor, AActor* OtherActor)
{
	if (OtherActor != nullptr && OtherActor != this)
	{
		class ACapstoneCharacter* MyCharacter = Cast<ACapstoneCharacter>(OtherActor);

		if (MyCharacter)
		{
			MyCharacter->UpdateHealth(-20.0f);
			Destroy();
		}
	}
}
