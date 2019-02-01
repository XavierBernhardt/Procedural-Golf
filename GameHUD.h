// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameFramework/PlayerController.h"
#include "GameHUD.generated.h"

/**
 * 
 */
UCLASS(config = Game)
class AGameHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	AGameHUD();

	UPROPERTY()
		UFont* HUDFont;

	float mouseX;
	float mouseY;

	void leftClick(float mouseX, float mouseY);
	void updateMouse(float mouseX, float mouseY);
	float hx = 0.f;
	float hy = 0.f;
	virtual void DrawHUD() override;

	struct button {
		float x1,y1,x2,y2; //Top Left x1,y1 | Bottom Right x2,y2
		FString col1, col2; //Hex colour | Normal col1 | Hovered col2
	};

	//Buttons
	//aware this isn't the fastest approach but its fine for a simple main menu

	button b1 = button{ 50, 300, 150, 400, "262626" , "999999" }; //button 1 sizes
	bool h1; //button 1 hovered over


};


