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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim Parameter", meta = (AllowPrivateAccess = "true"))
	float MouseSensitivity = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim Parameter", meta = (AllowPrivateAccess = "true"))
	float MouseAimSensitivity = 0.4f;


	bool bShootable = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Action", meta = (AllowPrivateAccess = "true"))
	bool bNowAiming = false;
	// this is used for Aiming start and end. using Camera Arm distance lerping (min 0, max 1)
	float AimingPercent = 0.f;

	float DefaultArmLength = 120.f;
	float AimArmLength = 60.f;
	float AimUpArmLength = -60.0f;
	float AimDownArmLength = 90.f;

	FVector DefaultSocketPosition = FVector(0.f, 30.f, 75.f);
	FVector AimSocketPosition = FVector(0.f, 50.f, 70.f);
	FVector AimUpSocketPosition = FVector(-45.f, 40.f, 40.f);
	FVector AimDownSocketPosition = FVector(0.f, 40.f, 35.f);

	void AimingLerp(float deltaTime);
	void AimingPitchLerp(float deltaTime);


	float moveXInput = 0.f;
	float moveYInput = 0.f;
	void SetMoveInputLerp(float aimmoveXInput, float aimmoveYInput);

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
	float GetAimPitch() const;


	UFUNCTION(BlueprintCallable)
	bool GetIsShootable() const { return bShootable; }

};

