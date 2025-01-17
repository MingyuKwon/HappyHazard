// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class UBoxComponent;

UCLASS()
class HAPPYHAZARD_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<UBoxComponent> CollisionBox;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
