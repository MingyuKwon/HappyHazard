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
class APlayerHUD;
struct FInputActionValue;
class AHappyPlayerController;
class AWeapon;

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

	/** Shoot Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ShiftAction;



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

	/** Called for Fire input */
	void ShiftStart(const FInputActionValue& Value);

	/** Called for Fire input */
	void ShiftEnd(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Action", meta = (AllowPrivateAccess = "true"))
	bool bNowShifting = false;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movemnet Parameter", meta = (AllowPrivateAccess = "true"))
	float AimMoveSpeed = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movemnet Parameter", meta = (AllowPrivateAccess = "true"))
	float DefaultMoveSpeed = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movemnet Parameter", meta = (AllowPrivateAccess = "true"))
	float ShiftMoveSpeed = 300.f;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim Parameter", meta = (AllowPrivateAccess = "true"))
	float MouseSensitivity = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim Parameter", meta = (AllowPrivateAccess = "true"))
	float MouseAimSensitivity = 0.4f;


	bool bShootable = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Action", meta = (AllowPrivateAccess = "true"))
	bool bNowAiming = false;


	// this is used for Aiming start and end. using Camera Arm distance lerping (min 0, max 1)
	float AimingPercent = 0.f;

	float DefaultArmLength = 130.f;
	float AimArmLength = 70.f;
	float AimUpArmLength = -50.0f;
	float AimDownArmLength = 100.f;

	FVector DefaultSocketPosition = FVector(0.f, 40.f, 65.f);
	FVector AimSocketPosition = FVector(5.f, 45.f, 64.f);
	FVector AimUpSocketPosition = FVector(-45.f, 40.f, 40.f);
	FVector AimDownSocketPosition = FVector(0.f, 40.f, 35.f);

	FRotator DefaultCameraRotation = FRotator(0.f, 0.f, 0.f);
	FRotator AimCameraRotation = FRotator(0.f, -40.f, 0.f);


	void AimingLerp(float deltaTime);
	void AimingPitchLerp(float deltaTime);


	float moveXInput = 0.f;
	float moveYInput = 0.f;
	void SetMoveInputLerp(float aimmoveXInput, float aimmoveYInput);

	void SetMoveSpeed();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	APlayerHUD* PlayerHUD;

	AHappyPlayerController* HappyPlayerController;

	AWeapon* EquipWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class Parameter", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> PistolClass;

	void SetWeaponEquip(bool isEquiped);

	bool bEquiped = false;


	void SetUIUpdateTick();

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

