// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "InteractionInterface.h"
#include "AttackActor.h"
#include "CapstoneCharacter.generated.h"

UCLASS(config=Game)
class ACapstoneCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	ACapstoneCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
		float maxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
		float health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
		float healthPercent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
		float maxMana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
		float mana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
		float manaPercent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
		float projectileSpeed;

	UPROPERTY(EditAnywhere)
		UBoxComponent* interactionBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
		float questStage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
		float killCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
		float maxKillCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
		TSubclassOf<AAttackActor> ProjectileOBJ;
protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface
	virtual void BeginPlay() override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintPure, Category = "Health")
		float GetHealth();

	UFUNCTION(BlueprintPure, Category = "Mana")
		float GetMana();

	UFUNCTION(BlueprintPure, Category = "Health")
		FText GetHealthText();

	UFUNCTION(BlueprintPure, Category = "Mana")
		FText GetManaText();

	UFUNCTION()
		void UpdateMana(float manaChange_);

	UFUNCTION()
		void UpdateHealth(float healthChange_);

	UFUNCTION()
		void Attack();

	UFUNCTION()
		void Interact();

	UFUNCTION()
		void UpdateQuestUI(float questStage_);

	UFUNCTION()
		float GetQuestUIStage();

	UFUNCTION()
		void UpdateKills(float killCount_);

	UFUNCTION()
		float GetKills();

	UFUNCTION()
		float GetMaxKills();

	UFUNCTION()
		void Death();

	//UFUNCTION(BlueprintCallable, Category = "CollisionEvents")
	//	void InteractonBoxBeginOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//UFUNCTION(BlueprintCallable, Category = "CollisionEvents")
	//	void InteractonBoxEndOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	IInteractionInterface* interface = nullptr;
};

