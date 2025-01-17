// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/HappyHazardCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "UI/PlayerHUD.h"
#include "Controller/HappyPlayerController.h"
#include "Battle/Weapon.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AHappyHazardCharacter

AHappyHazardCharacter::AHappyHazardCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SocketOffset = FVector(0.f, 30.f, 75.f);
	CameraBoom->TargetArmLength = 120.f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AHappyHazardCharacter::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (GEngine)
	{
		FString text = FString::Printf(TEXT("MovementVector moveXInput %f"), moveXInput);
		GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Blue, text);
	}

	if (GEngine)
	{
		FString text = FString::Printf(TEXT("MovementVector moveYInput %f"), moveYInput);
		GEngine->AddOnScreenDebugMessage(2, 0.f, FColor::Red, text);
	}

	if (GetCharacterMovement()->Velocity.Length() <= 0)
	{
		SetMoveInputLerp(0.f, 0.f);
	}


	AimingLerp(deltaTime);
	AimingPitchLerp(deltaTime);

	SetUIUpdateTick();

	SetMoveSpeed();

	if (GetIsAiming())
	{
		FRotator NewRotation = Controller->GetControlRotation();
		NewRotation.Pitch = 0;
		SetActorRotation(NewRotation);
	}


}

void AHappyHazardCharacter::AimingLerp(float deltaTime)
{
	if (GetIsAiming())
	{
		AimingPercent += deltaTime * 5;

	}
	else
	{
		AimingPercent -= deltaTime * 5;
	
	}

	AimingPercent = FMath::Clamp(AimingPercent, 0.f, 1.f);
	bShootable = (AimingPercent >= 0.99f);
	

	float LerpArmLength = FMath::Lerp(DefaultArmLength, AimArmLength, AimingPercent);
	FVector LerpSocketPosition = FMath::Lerp(DefaultSocketPosition, AimSocketPosition, AimingPercent);
	//FRotator LerpCameraRotator = FMath::Lerp(DefaultCameraRotation, AimCameraRotation, AimingPercent);

	CameraBoom->TargetArmLength = LerpArmLength;
	CameraBoom->SocketOffset = LerpSocketPosition;

	//FollowCamera->SetRelativeRotation(LerpCameraRotator);


}

void AHappyHazardCharacter::AimingPitchLerp(float deltaTime)
{
	if (!GetIsAiming()) return;

	float Pitch = GetAimPitch();

	Pitch = Pitch / 90;

	if (Pitch >= 0)
	{
		float LerpArmLength = FMath::Lerp(AimArmLength, AimUpArmLength, Pitch);
		CameraBoom->TargetArmLength = LerpArmLength;

		FVector LerpSocketPosition = FMath::Lerp(AimSocketPosition, AimUpSocketPosition, Pitch);
		CameraBoom->SocketOffset = LerpSocketPosition;

	}
	else
	{
		Pitch = FMath::Abs(Pitch);
		float LerpArmLength = FMath::Lerp(AimArmLength, AimDownArmLength, Pitch);
		CameraBoom->TargetArmLength = LerpArmLength;

		FVector LerpSocketPosition = FMath::Lerp(AimSocketPosition, AimDownSocketPosition, Pitch);
		CameraBoom->SocketOffset = LerpSocketPosition;

	}
}

void AHappyHazardCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	if (GetController())
	{
		HappyPlayerController = Cast<AHappyPlayerController>(GetController());
	}

	if (HappyPlayerController)
	{
		PlayerHUD = Cast<APlayerHUD>(HappyPlayerController->GetHUD());
	}
}

void AHappyHazardCharacter::SetWeaponEquip(bool isEquiped)
{
	bEquiped = isEquiped;

	if (EquipWeapon)
	{
		EquipWeapon->Destroy();
		EquipWeapon = nullptr;
	}

	if (bEquiped)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		if (PistolClass)
		{
			EquipWeapon = GetWorld()->SpawnActor<AWeapon>(PistolClass, SpawnParams);
		}

		if (EquipWeapon)
		{
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
			EquipWeapon->AttachToComponent(GetMesh(), AttachmentRules, FName("PistolSocket"));
		}
	}
}

void AHappyHazardCharacter::SetUIUpdateTick()
{
	if (PlayerHUD)
	{
		PlayerHUD->SetAimDisplay(GetIsAiming());
	}

}

bool AHappyHazardCharacter::GetIsAiming() const
{
	return bNowAiming && !GetCharacterMovement()->IsFalling(); 
}

float AHappyHazardCharacter::GetMoveXInput() const
{
	return moveXInput;
}

float AHappyHazardCharacter::GetMoveYInput() const
{
	return moveYInput;
}

float AHappyHazardCharacter::GetAimPitch() const
{
	float Pitch = 0.f;

	if (Controller)
	{
		Pitch = Controller->GetControlRotation().Pitch;
		if (Pitch >= 180)
		{
			Pitch -= 360;
		}
		Pitch = FMath::Clamp(Pitch, -90.f, 90.f);
	
	}
	
	return Pitch;
}

//////////////////////////////////////////////////////////////////////////
// Input


void AHappyHazardCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHappyHazardCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AHappyHazardCharacter::Look);

		// Fire
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AHappyHazardCharacter::Fire);


		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &AHappyHazardCharacter::AimStart);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &AHappyHazardCharacter::AimEnd);

		EnhancedInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AHappyHazardCharacter::ShiftStart);
		EnhancedInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AHappyHazardCharacter::ShiftEnd);

		
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AHappyHazardCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);


		float moderateValue = (MovementVector.Length() > 1) ? 0.5f : 1;
		
		SetMoveInputLerp(MovementVector.X * moderateValue, MovementVector.Y * moderateValue);
	}

}

void AHappyHazardCharacter::SetMoveInputLerp(float aimmoveXInput, float aimmoveYInput)
{
	if (aimmoveXInput * moveXInput < 0)
	{
		moveXInput = FMath::Lerp(moveXInput, aimmoveXInput, 0.03f);
	}
	else
	{
		moveXInput = FMath::Lerp(moveXInput, aimmoveXInput, 0.1f);
	}

	if (aimmoveYInput * moveYInput < 0)
	{
		moveYInput = FMath::Lerp(moveYInput, aimmoveYInput, 0.03f);
	}
	else
	{
		moveYInput = FMath::Lerp(moveYInput, aimmoveYInput, 0.1f);
	}
}

void AHappyHazardCharacter::SetMoveSpeed()
{
	float Speed = DefaultMoveSpeed;

	if (bNowAiming)
	{
		Speed = AimMoveSpeed;
	}
	else if (bNowShifting)
	{
		Speed = ShiftMoveSpeed;
	}

	GetCharacterMovement()->MaxWalkSpeed = Speed;

}

void AHappyHazardCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (bNowAiming)
	{
		LookAxisVector = LookAxisVector * MouseAimSensitivity;
	}
	else
	{
		LookAxisVector = LookAxisVector * MouseSensitivity;
	}

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AHappyHazardCharacter::AimStart(const FInputActionValue& Value)
{
	bNowAiming = true;

	GetCharacterMovement()->bOrientRotationToMovement = false; 
	SetWeaponEquip(true);

}

void AHappyHazardCharacter::AimEnd(const FInputActionValue& Value)
{
	bNowAiming = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 400.0f; 

	SetWeaponEquip(false);

}

void AHappyHazardCharacter::Fire(const FInputActionValue& Value)
{
	if (!bShootable) return;

 	UE_LOG(LogTemp, Warning, TEXT("Fire"));
}


void AHappyHazardCharacter::ShiftStart(const FInputActionValue& Value)
{
	bNowShifting = true;
}

void AHappyHazardCharacter::ShiftEnd(const FInputActionValue& Value)
{
	bNowShifting = false;
}
