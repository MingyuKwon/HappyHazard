// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class UBoxComponent;
class UNiagaraSystem;
class UAnimMontage;

UCLASS()
class HAPPYHAZARD_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<UBoxComponent> CollisionBox;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(EditAnywhere, Category = "Fire Para")
	UNiagaraSystem* MuzzleEffect;

	UPROPERTY(EditAnywhere, Category = "Fire Para")
	UNiagaraSystem* BulletHitImpact;

	UPROPERTY(EditAnywhere, Category = "Fire Para")
	UNiagaraSystem* BulletTrail;



	UPROPERTY(EditAnywhere, Category = "Fire Para")
	UAnimMontage* FireMontage;



public:	
	void Fire(FVector CameraPosition, FVector CameraNormalVector);

};
