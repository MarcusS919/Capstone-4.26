// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelChanger.h"
#include "CapstoneCharacter.h"
#include "UObject/UObjectIterator.h"

// Sets default values
ALevelChanger::ALevelChanger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	questCompleteStage = 3.0f;
}

// Called when the game starts or when spawned
void ALevelChanger::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &ALevelChanger::OnOverlap);
}

// Called every frame
void ALevelChanger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelChanger::OnOverlap(AActor* MyOverlappedActor, AActor* OtherActor)
{
	if (OtherActor != nullptr && OtherActor != this)
	{
		class ACapstoneCharacter* MyCharacter = Cast<ACapstoneCharacter>(OtherActor);
		if (MyCharacter)
		{
			for (TObjectIterator<ACapstoneCharacter> Itr; Itr; ++Itr)
			{
				if (Itr->IsA(ACapstoneCharacter::StaticClass()))
				{
					ACapstoneCharacter* actorClass = *Itr;
					if (actorClass->GetQuestUIStage() == questCompleteStage) {
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("quest changed||quest stage: %f"), actorClass->GetQuestUIStage()));
						FName levelName = FName(*levelString);
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("loading new level")));
						float temp;
						temp = actorClass->GetQuestUIStage();
						actorClass->UpdateQuestUI(-temp);
						temp = actorClass->GetKills();
						actorClass->UpdateKills(-temp);
						UGameplayStatics::OpenLevel(GetWorld(), levelName);
					}
					actorClass = nullptr;
				}
			}

		}
	}
}
