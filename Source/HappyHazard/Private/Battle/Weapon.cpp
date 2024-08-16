// Fill out your copyright notice in the Description page of Project Settings.


#include "Battle/Weapon.h"
#include "Components/BoxComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	CollisionBox = CreateDefaultSubobject<UBoxComponent>(FName("CollisionBox"));
	RootComponent = CollisionBox;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeapon::Fire()
{
    UE_LOG(LogTemp, Warning, TEXT("Fire"));

    if (WeaponMesh && MuzzleEffect)
    {
        FVector MuzzleLocation = WeaponMesh->GetSocketLocation(FName("Muzzle"));
        FRotator MuzzleRotation = WeaponMesh->GetSocketRotation(FName("Muzzle"));

        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            MuzzleEffect,
            MuzzleLocation,
            MuzzleRotation
        );
    }

    if (FireMontage && WeaponMesh)
    {
        UAnimInstance* AnimInstance = WeaponMesh->GetAnimInstance();
        if (AnimInstance)
        {
            AnimInstance->Montage_Play(FireMontage);
        }
    }
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

