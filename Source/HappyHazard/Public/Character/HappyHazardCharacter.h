// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "HappyHazardCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AHappyHazardCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Aim Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AimAction;

	/** Shoot Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* FireAction;


public:
	AHappyHazardCharacter();
	
	virtual void Tick(float deltaTime) override;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for Aim input */
	void AimStart(const FInputActionValue& Value);

	/** Called for Aim input */
	void AimEnd(const FInputActionValue& Value);

	/** Called for Fire input */
	void Fire(const FInputActionValue& Value);


	bool bShootable = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Action", meta = (AllowPrivateAccess = "true"))
	bool bNowAiming = false;
	// this is used for Aiming start and end. using Camera Arm distance lerping (min 0, max 1)
	float AimingPercent = 0.f;

	float DefaultArmLength = 120.f;
	float AimArmLength = 70.f;

	FVector DefaultSocketPosition = FVector(0.f, 30.f, 75.f);
	FVector AimSocketPosition = FVector(0.f, 50.f, 70.f);

	void AimingLerp(float deltaTime);

	float moveXInput = 0.f;
	float moveYInput = 0.f;


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	/** Returns Is Player is Aiming **/
	UFUNCTION(BlueprintCallable)
	bool GetIsAiming() const;

	UFUNCTION(BlueprintCallable)
	float GetMoveXInput() const;

	UFUNCTION(BlueprintCallable)
	float GetMoveYInput() const;

	UFUNCTION(BlueprintCallable)
	bool GetIsShootable() const { return bShootable; }

};

