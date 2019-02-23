// Fill out your copyright notice in the Description page of Project Settings.

#include "GameHUD.h"
#include "PlayerPawn.h"
#include "GameModeCPP.h"
#include "Engine/Canvas.h"
#include "Engine/Font.h"
#include "Engine.h"
#include "CanvasItem.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "PlayerController1CPP.h"

AGameHUD::AGameHUD() {
	static ConstructorHelpers::FObjectFinder<UFont> Font(TEXT("/Engine/EngineFonts/RobotoDistanceField"));
	HUDFont = Font.Object;
}


void AGameHUD::leftClick(float mouseX, float mouseY)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, FString::Printf(TEXT("GAMEHUD: Left Click at: x%f, y%f"), mouseX, mouseY));

}

void AGameHUD::updateMouse(float mouseX, float mouseY)
{
	GEngine->AddOnScreenDebugMessage(300, 99.0f, FColor::White, FString::Printf(TEXT("GAMEHUD: Mouse Location at: x%f, y%f"), mouseX, mouseY));
	
	if (Canvas)
	{	
		hx = Canvas->SizeX / 1280.f;
		hy = Canvas->SizeY / 720.f;
	}

	h1 = false;

	//if (mouseX > hx * b1.x1	&& mouseY > hy * b1.y1 && mouseX < hx * b1.x1+100 && mouseY < hy * b1.y1+100)
	if (mouseX > b1.x1	&& mouseY > b1.y1 && mouseX < b1.x1 + 100 && mouseY < b1.y1 + 100)
		h1 = true;
}

void AGameHUD::DrawHUD()
{
	Super::DrawHUD();
	if (Canvas)
	{
		hx = Canvas->SizeX / 1280.f;
		hy = Canvas->SizeY / 720.f;
	}
	FVector2D ScaleVec(hy * 1.4f, hy * 1.4f);



	FString LevelName = GetWorld()->GetMapName();
	LevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

	if (LevelName.Equals("MainMenu")) {
		
		//	APlayerController * Controller = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		//	if (Controller->GetName() == FString("PlayerController1_C_0")) {
		//	//Drawing Buttons
		//
		//	//Button 1 Box
		//	/*
		//		FCanvasTileItem btn1(FVector2D(hx * b1.x1, hy * b1.y1), FVector2D(hx * b1.x1+100, hy * b1.y1+100), (FColor::FromHex(b1.col1)));
		//	if (h1) 
		//		btn1 = FCanvasTileItem(FVector2D(hx * b1.x1, hy * b1.y1), FVector2D(hx * b1.x1+100, hy * b1.y1+100), (FColor::FromHex(b1.col2)));*/
		//	FCanvasTileItem btn1(FVector2D(b1.x1, b1.y1), FVector2D(b1.x1+100, b1.y1+100), (FColor::FromHex(b1.col1)));
		//	if (h1) 
		//		btn1 = FCanvasTileItem(FVector2D(b1.x1, b1.y1), FVector2D(b1.x1+100, b1.y1+100), (FColor::FromHex(b1.col2)));
		//	Canvas->DrawItem(btn1);

		//	//Button 1 Text
		//	FCanvasTextItem btn1Text(FVector2D(hx * b1.x1+5, hy * b1.y1+5), FText::FromString(FString("Maze Generation")), HUDFont, FLinearColor::White);
		//	btn1Text.Scale = ScaleVec * 0.8;
		//	btn1Text.bOutlined = true;
		//	btn1Text.OutlineColor = FColor::Black;
		//	btn1Text.EnableShadow(FColor::Black, (FVector2D(1.f, 1.f)));
		//	Canvas->DrawItem(btn1Text);


		//	//Title
		//	FCanvasTextItem GolfTitle(FVector2D(hx * 50.f, hy * 50.f), FText::FromString(FString("Procedural Golf")), HUDFont, FLinearColor::White);
		//	GolfTitle.Scale = ScaleVec * 2;
		//	GolfTitle.bOutlined = true;
		//	GolfTitle.OutlineColor = FColor::Black;
		//	GolfTitle.EnableShadow(FColor::Black, (FVector2D(1.f, 1.f)));
		//	Canvas->DrawItem(GolfTitle);
		//}
		///*
		//		FString string = "controller name : ";
		//string.Append((Controller->GetName()));
		//FCanvasTextItem controllerName(FVector2D(hx * 36.f, hy * 570.f), FText::FromString(string), HUDFont, FLinearColor::White);
		//controllerName.Scale = ScaleVec * 0.76;
		//controllerName.bOutlined = true;
		//controllerName.OutlineColor = FColor::Black;
		//controllerName.EnableShadow(FColor::Black, (FVector2D(1.f, 1.f)));
		//Canvas->DrawItem(controllerName);

		//*/
		
		

	}
	else {
		if (LevelName.Equals("MazeGeneration")) {
		}
		else if (LevelName.Equals("ControlMap")) {
		}
		else if (LevelName.Equals("SnakeGeneration")) {
		}

		//draw player hud if player is on screen
		APlayerPawn * PlayerPawn = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		if ((PlayerPawn != nullptr))
		{

			if (PlayerPawn->canShoot) {
				//FCanvasBoxItem ForceBoxItem2(FVector2D(HUDXRatio * 35.f, HUDYRatio * 620.f), FVector2D(((PlayerPawn->force)-5)*3, 10.f));
				//ForceBoxItem2.SetColor(FLinearColor::White);
				//ForceBoxItem2.LineThickness = 0.f;
				//Canvas->DrawItem(ForceBoxItem2);



				//FCanvasTileItem ForceBoxBack(HUDXRatio * 35.f, HUDYRatio * 620.f, PlayerPawn->force*5, 20.f, 0,f, 0.f,  );


				//FCanvas::DrawTile(HUDXRatio * 35.f, HUDYRatio * 620.f, PlayerPawn->force * 5, 20.f, 0.f);
				//FCanvasTileItem ForceTileItemBack(FVector2D(HUDXRatio * 35.f, HUDYRatio * 620.f), FVector2D(135.f, 10.f), FLinearColor(42.f, 42.f, 42.f, 0.5f));
				//Canvas->DrawItem(ForceTileItemBack);

				//FCanvasTileItem ForceTileItemBar(FVector2D(HUDXRatio * 35.f, HUDYRatio * 620.f), FVector2D(((PlayerPawn->force) - 5) * 3, 10.f), FLinearColor(66.f, 150.f, 16.f, 0.8f));
				//Canvas->DrawItem(ForceTileItemBar);

				//FCanvasBoxItem ForceTileItemOutline(FVector2D(HUDXRatio * 35.f, HUDYRatio * 620.f), FVector2D(135.f, 10.f));
				//ForceTileItemOutline.SetColor(FLinearColor(23.f, 23.f, 23.f, 0.8f));;
				//ForceTileItemOutline.LineThickness = 2.f;
				//Canvas->DrawItem(ForceTileItemOutline);

				//FCanvasTileItem ForceTileItemBack(FVector2D(HUDXRatio * 35.f, HUDYRatio * 650.f), FVector2D(135.f*2, 10.f*3), (FColor::FromHex("262626")));
				FCanvasTileItem ForceTileItemBack(FVector2D(hx * 35.f, hy * 635.f), FVector2D(hx * 400, hy * 50), (FColor::FromHex("262626")));
				Canvas->DrawItem(ForceTileItemBack);

				//FCanvasTileItem ForceTileItemBar(FVector2D(HUDXRatio * 35.f, HUDYRatio * 650.f), FVector2D(((PlayerPawn->force) - 5) * 2 *3, 10.f*3), (FColor::FromHex("42a010")));
				FCanvasTileItem ForceTileItemBar(FVector2D(hx * 35.f, hy * 635.f), FVector2D(hx *  (PlayerPawn->force - 5) * 8.9, hy * 50), (FLinearColor::LerpUsingHSV(FColor::FromHex("b50e0e"), (FLinearColor::LerpUsingHSV(FColor::FromHex("b50e0e"), FColor::FromHex("42a010"), (PlayerPawn->force - 5) * 2.7 / 100)), (PlayerPawn->force - 5) * 2 / 100)));// ("42a010")));
				Canvas->DrawItem(ForceTileItemBar);

				FCanvasBoxItem ForceTileItemOutline(FVector2D(hx * 35.f, hy * 635.f), FVector2D(hx * 400, hy * 50));
				ForceTileItemOutline.SetColor((FColor::FromHex("1a1a1a")));
				ForceTileItemOutline.LineThickness = 3.f;
				Canvas->DrawItem(ForceTileItemOutline);



				FCanvasTextItem ForceTextItem(FVector2D(hx * 36.f, hy * 570.f), FText::FromString(FString("Left Mouse:   Shoot\nRight Mouse: Change Shot Force & Angle")), HUDFont, FLinearColor::White);
				ForceTextItem.Scale = ScaleVec * 0.76;
				ForceTextItem.bOutlined = true;
				ForceTextItem.OutlineColor = FColor::Black;
				ForceTextItem.EnableShadow(FColor::Black, (FVector2D(1.f, 1.f)));
				Canvas->DrawItem(ForceTextItem);
				//PlayerPawn->ForceDisplayString


			}

			FString ShotsTakenString = FString(TEXT("Current Score: "));
			ShotsTakenString.Append(FString::FormatAsNumber(PlayerPawn->score));
			ShotsTakenString.Append(FString(TEXT("\nShots Taken: ")));
			ShotsTakenString.Append(FString::FormatAsNumber(PlayerPawn->shotsTaken));



			FString MapInfoString = FString(TEXT("Total Holes: "));
			MapInfoString.Append(FString::FormatAsNumber(PlayerPawn->totalHoles));
			MapInfoString.Append(FString(TEXT   ("   \nHole Par: ")));
			MapInfoString.Append(FString::FormatAsNumber(PlayerPawn->holePar));
			MapInfoString.Append (FString(TEXT  ("\nCurrent Hole: ")));
			MapInfoString.Append(FString::FormatAsNumber(PlayerPawn->CurrentHole + 1));

			FCanvasTextItem ForceTextItem(FVector2D(hx * 36, hy * 36), FText::FromString(ShotsTakenString), HUDFont, FLinearColor::White);
			ForceTextItem.Scale = ScaleVec * 1;
			ForceTextItem.bOutlined = true;
			ForceTextItem.OutlineColor = FColor::Black;
			ForceTextItem.EnableShadow(FColor::Black, (FVector2D(1.f, 1.f)));
			Canvas->DrawItem(ForceTextItem);


			FCanvasTextItem MapInfoItem(FVector2D(Canvas->SizeX * 0.80, hy * 36), FText::FromString(MapInfoString), HUDFont, FLinearColor::White);
			MapInfoItem.Scale = ScaleVec * 1;
			MapInfoItem.bOutlined = true;
			MapInfoItem.OutlineColor = FColor::Black;
			MapInfoItem.EnableShadow(FColor::Black, (FVector2D(1.f, 1.f)));
			Canvas->DrawItem(MapInfoItem);

			if (PlayerPawn->DrawFlagHitText) {

				//FString FlagHitString = FString(TEXT("Goal Reached! \nShots Taken: "));
				//FlagHitString.Append(FString::FormatAsNumber(PlayerPawn->shotsTaken));




				FString FlagHitString = FString(TEXT("Goal Reached!"));
				if (PlayerPawn->shotsTaken == 1) {
					FlagHitString = FString(TEXT("Hole in One!"));
				}
				if (PlayerPawn->hitTotalHoles) {
					FlagHitString = FString(TEXT("FINAL HOLE REACHED!\nYour Score: "));
					FlagHitString.Append(FString::FormatAsNumber(PlayerPawn->score));
				}


				//FCanvasTextItem FlagHitText(FVector2D(hx * 640.f + 180, hy * 180), FText::FromString(FlagHitString), HUDFont, FLinearColor::Yellow);
				FCanvasTextItem FlagHitText(FVector2D((Canvas->SizeX * 0.5), hy * 180), FText::FromString(FlagHitString), HUDFont, FLinearColor::Yellow);

				if (PlayerPawn->hitTotalHoles) {
					FlagHitText = FCanvasTextItem(FVector2D((Canvas->SizeX * 0.65), hy * 180), FText::FromString(FlagHitString), HUDFont, FLinearColor::Yellow);
				}

				FlagHitText.Scale = ScaleVec * 2.5;
				FlagHitText.bCentreX = true;
				FlagHitText.bCentreY = true;
				Canvas->bCenterX = true;
				FlagHitText.bOutlined = true;
				FlagHitText.OutlineColor = FColor::Black;
				FlagHitText.EnableShadow(FColor::Black, (FVector2D(1.f, 1.f)));
				Canvas->DrawItem(FlagHitText);
			}
		}
	}
}