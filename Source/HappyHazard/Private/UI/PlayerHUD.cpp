// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUD.h"
#include "UI/AimCrossHairWidget.h"

void APlayerHUD::DrawHUD()
{
	Super::DrawHUD();


}

void APlayerHUD::BeginPlay()
{
    Super::BeginPlay();

    if (AimCrossHairWidgetClass != nullptr)
    {
        UAimCrossHairWidget* HUDWidget = CreateWidget<UAimCrossHairWidget>(GetWorld(), AimCrossHairWidgetClass);
        if (HUDWidget != nullptr)
        {
            HUDWidget->AddToViewport();
        }
    }
}