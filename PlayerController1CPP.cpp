// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerController1CPP.h"
#include "GameHUD.h"
#include "Engine/World.h"

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

//APlayerController1CPP::APlayerController1CPP()
//{
	//bShowMouseCursor = true;
	//bEnableClickEvents = true;
	//bEnableMouseOverEvents = true;
//}

//void APlayerController1CPP::Tick(float DeltaTime)
//{
//	AGameHUD * HUD = Cast<AGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
//	GetMousePosition(mouseX, mouseY);
//	HUD->updateMouse(mouseX, mouseY);
//	//UE_LOG(LogTemp, Warning, TEXT("Mouse Location: %f, %f"), mouseX, mouseY);
//}
//
//void APlayerController1CPP::SetupInputComponent()
//{
//	Super::SetupInputComponent();
//	InputComponent->BindAction("Shoot", IE_Pressed, this, &APlayerController1CPP::LeftClick);
//	InputComponent->BindAction("Shoot", IE_Released, this, &APlayerController1CPP::LeftRelease);
//}
//
//void APlayerController1CPP::LeftClick()
//{
//	AGameHUD * HUD = Cast<AGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
//	HUD->leftClick(mouseX, mouseY);
//}
//
//void APlayerController1CPP::LeftRelease()
//{
////	UE_LOG(LogTemp, Warning, TEXT("Left release at: %f, %f"), mouseX, mouseY);
//
//}
