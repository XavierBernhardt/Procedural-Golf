// Fill out your copyright notice in the Description page of Project Settings.

#include "GameHUD.h"
#include "PlayerPawn.h"
#include "GameModeCPP.h"
#include "Engine/Canvas.h"
#include "Engine/Font.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"

AGameHUD::AGameHUD() {
	static ConstructorHelpers::FObjectFinder<UFont> Font(TEXT("/Engine/EngineFonts/RobotoDistanceField"));
	HUDFont = Font.Object;
}


void AGameHUD::DrawHUD()
{
	Super::DrawHUD();

	// Calculate ratio from 720p
	const float HUDXRatio = Canvas->SizeX / 1280.f;
	const float HUDYRatio = Canvas->SizeY / 720.f;

		// Get our vehicle so we can check if we are in car. If we are we don't want onscreen HUD
	APlayerPawn * PlayerPawn = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if ((PlayerPawn != nullptr))
		{
			FVector2D ScaleVec(HUDYRatio * 1.4f, HUDYRatio * 1.4f);

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
				FCanvasTileItem ForceTileItemBack(FVector2D(HUDXRatio * 35.f, HUDYRatio * 635.f), FVector2D(HUDXRatio*400, HUDYRatio*50), (FColor::FromHex("262626")));
				Canvas->DrawItem(ForceTileItemBack);

				//FCanvasTileItem ForceTileItemBar(FVector2D(HUDXRatio * 35.f, HUDYRatio * 650.f), FVector2D(((PlayerPawn->force) - 5) * 2 *3, 10.f*3), (FColor::FromHex("42a010")));
				FCanvasTileItem ForceTileItemBar(FVector2D(HUDXRatio * 35.f, HUDYRatio * 635.f), FVector2D(HUDXRatio *  (PlayerPawn->force - 5) * 8.9, HUDYRatio * 50), (FLinearColor::LerpUsingHSV( FColor::FromHex("b50e0e"), (FLinearColor::LerpUsingHSV(FColor::FromHex("b50e0e"), FColor::FromHex("42a010"), (PlayerPawn->force - 5) * 2.7 / 100)),(PlayerPawn->force-5)*2/100)));// ("42a010")));
				Canvas->DrawItem(ForceTileItemBar); 

				FCanvasBoxItem ForceTileItemOutline(FVector2D(HUDXRatio * 35.f, HUDYRatio * 635.f), FVector2D(HUDXRatio * 400, HUDYRatio * 50)); 
				ForceTileItemOutline.SetColor((FColor::FromHex("1a1a1a")));
				ForceTileItemOutline.LineThickness = 3.f;
				Canvas->DrawItem(ForceTileItemOutline);



				FCanvasTextItem ForceTextItem(FVector2D(HUDXRatio * 36.f, HUDYRatio * 570.f), FText::FromString(FString("Left Mouse:   Shoot\nRight Mouse: Change Shot Force & Angle")), HUDFont, FLinearColor::White);
				ForceTextItem.Scale = ScaleVec*0.76;
				ForceTextItem.bOutlined = true;
				ForceTextItem.OutlineColor = FColor::Black;
				ForceTextItem.EnableShadow(FColor::Black, (FVector2D(1.f, 1.f)));
				Canvas->DrawItem(ForceTextItem);
				//PlayerPawn->ForceDisplayString


			}

			FString ShotsTakenString = FString(TEXT("Current Hole: "));
			ShotsTakenString.Append(FString::FormatAsNumber(PlayerPawn->CurrentHole+1));
			ShotsTakenString.Append(FString(TEXT("\nShots Taken: ")));
			ShotsTakenString.Append(FString::FormatAsNumber(PlayerPawn->shotsTaken));


			FCanvasTextItem ForceTextItem(FVector2D(HUDXRatio * 36, HUDYRatio * 36), FText::FromString(ShotsTakenString), HUDFont, FLinearColor::White);
			ForceTextItem.Scale = ScaleVec * 1;
			ForceTextItem.bOutlined = true;
			ForceTextItem.OutlineColor = FColor::Black;
			ForceTextItem.EnableShadow(FColor::Black, (FVector2D(1.f, 1.f)));
			Canvas->DrawItem(ForceTextItem);

			if (PlayerPawn->DrawFlagHitText) {

				FString FlagHitString = FString(TEXT("Goal Reached! \nShots Taken: "));
				FlagHitString.Append(FString::FormatAsNumber(PlayerPawn->shotsTaken));

				FCanvasTextItem FlagHitText(FVector2D(HUDXRatio * 640.f + 180, HUDYRatio * 180), FText::FromString(FlagHitString), HUDFont, FLinearColor::Yellow);
				FlagHitText.Scale = ScaleVec * 1.8;
				FlagHitText.bCentreX = true;
				Canvas->bCenterX = true;
				FlagHitText.bOutlined = true;
				FlagHitText.OutlineColor = FColor::Black;
				FlagHitText.EnableShadow(FColor::Black, (FVector2D(1.f, 1.f)));
				Canvas->DrawItem(FlagHitText);
			}
	}

}
