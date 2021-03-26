// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "CapstoneCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// ACapstoneCharacter

ACapstoneCharacter::ACapstoneCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	maxHealth = 100.0f;
	health = maxHealth;
	healthPercent = 1.0f;

	maxMana = 100.0f;
	mana = maxMana;
	manaPercent = 1.0f;
	questStage = 0.0f;
	killCount = 0.0f;
	maxKillCount = 2.0f;

	projectileSpeed = 1000.0f;

	interactionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBox"));
	interactionBox->SetupAttachment(RootComponent);
}

//////////////////////////////////////////////////////////////////////////
// Input

void ACapstoneCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACapstoneCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACapstoneCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ACapstoneCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ACapstoneCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ACapstoneCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ACapstoneCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ACapstoneCharacter::OnResetVR);

	//Player Action
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ACapstoneCharacter::Attack);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ACapstoneCharacter::Interact);
}

void ACapstoneCharacter::BeginPlay()
{
	Super::BeginPlay();
	//interactionBox->OnComponentBeginOverlap.AddDynamic(this, &ACapstoneCharacter::InteractonBoxBeginOverlap);
	//interactionBox->OnComponentEndOverlap.AddDynamic(this, &ACapstoneCharacter::InteractonBoxEndOverlap);
	maxHealth = 100.0f;
	health = maxHealth;
	healthPercent = 1.0f;

	maxMana = 100.0f;
	mana = maxMana;
	manaPercent = 1.0f;
	questStage = 0.0f;
	killCount = 0.0f;
	maxKillCount = 2.0f;

	projectileSpeed = 1000.0f;
}

void ACapstoneCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	TArray<AActor*>overlappingActors;
	interactionBox->GetOverlappingActors(overlappingActors);

	if (overlappingActors.Num() == 0) {
		if (interface) {
			interface->HideInteractionWidget();
			interface = nullptr;
		}
		return;
	}
	AActor* closestActor = overlappingActors[0];

	for (auto currentActor : overlappingActors) {
		if (GetDistanceTo(currentActor) < GetDistanceTo(closestActor)) {
			closestActor = currentActor;
		}
	}

	if (interface) {
		interface->HideInteractionWidget();
	}

	interface = Cast<IInteractionInterface>(closestActor);

	if (interface) {
		interface->ShowInteractionWidget();
	}
}

float ACapstoneCharacter::GetHealth()
{
	return healthPercent;
}

float ACapstoneCharacter::GetMana()
{
	return manaPercent;
}

FText ACapstoneCharacter::GetHealthText()
{
	int hp = FMath::RoundHalfFromZero(healthPercent * 100);
	FString hpString = FString::FromInt(hp);
	hpString += FString(TEXT("%"));
	FText hpText = FText::FromString(hpString);
	return hpText;
}

FText ACapstoneCharacter::GetManaText()
{
	int manaInt = FMath::RoundHalfFromZero(manaPercent * 100);
	FString manaString = FString::FromInt(manaInt);
	manaString += FString(TEXT("%"));
	FText manaText = FText::FromString(manaString);
	return manaText;
}

void ACapstoneCharacter::UpdateMana(float manaChange_)
{
	mana = FMath::Clamp(mana += manaChange_, 0.0f, maxMana);
	manaPercent = mana / maxMana;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("mana: %f"), manaPercent));
}

void ACapstoneCharacter::UpdateHealth(float healthChange_)
{
	health = FMath::Clamp(health += healthChange_, 0.0f, maxHealth);
	healthPercent = health / maxHealth;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("health: %f"), healthPercent));

	if (healthPercent <= 0.0f) {
		Death();
	}
}

float ACapstoneCharacter::GetQuestUIStage()
{
	return questStage;
}

void ACapstoneCharacter::UpdateQuestUI(float questStage_)
{
	questStage += questStage_;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("quest changed||quest stage: %f"), questStage));
}

void ACapstoneCharacter::UpdateKills(float killCount_)
{
	killCount += killCount_;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("kills updated")));

	if (killCount == maxKillCount && questStage == 1.0f) {
		UpdateQuestUI(1.0f);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("quest: %f"), questStage));
	}
}

float ACapstoneCharacter::GetKills()
{
	return killCount;
}

float ACapstoneCharacter::GetMaxKills()
{
	return maxKillCount;
}

void ACapstoneCharacter::Death()
{
	if (healthPercent <= 0.0f) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("GameOver")));
		UGameplayStatics::OpenLevel(GetWorld(), "GameOverMenuLevel");
	}
}

void ACapstoneCharacter::Attack()
{
	if (Controller != NULL)
	{
		if (manaPercent > 0.0f) {
			float manaChange = -5.0f;
			UpdateMana(manaChange);
			auto Projectile = GetWorld()->SpawnActor<AAttackActor>(ProjectileOBJ, this->GetActorLocation(), this->GetActorRotation());
			// Call the LaunchProjectile function of our newly created Projectile with our tank's launch speed
			Projectile->ShootProjectile(projectileSpeed);
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Not enough mana")));
		}
	}
}

void ACapstoneCharacter::Interact() {
	if (interface) {
		interface->InteractWithMe();
	}
}

//void ACapstoneCharacter::InteractonBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	interface = Cast<IInteractionInterface>(OtherActor);
//	if (interface) {
//		interface->ShowInteractionWidget();
//	}
//}
//
//void ACapstoneCharacter::InteractonBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//	if (interface) {
//		interface->HideInteractionWidget();
//		interface = nullptr;
//	}
//}

void ACapstoneCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ACapstoneCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ACapstoneCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ACapstoneCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACapstoneCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ACapstoneCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ACapstoneCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
