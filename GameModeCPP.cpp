// Fill out your copyright notice in the Description page of Project Settings.

#include "GameModeCPP.h"
#include "Engine.h"
#include "Engine/World.h"

AGameModeCPP::AGameModeCPP() {
}

void AGameModeCPP::InitGameState()
{
	Super::InitGameState();

	AGameState* MyGameState = Cast<AGameState>(GameState);
	if (MyGameState)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("InitState"));

		CurrentHole = 0;

		//Code to fill the locations of each starting point
		//default set everything to 0.f
		for (int i = 0; i < 10; i++) {
			x[i] = 0.f;
			y[i] = 0.f;
			z[i] = 0.f;
			r[i] = 380.f; //a rotation that will never be used -> used here to check if a hole here exists
			// eg if next r[currenthole+1] == 380.f then there is no hole there and we have ended.
		}

		//Hole 0
		x[0] = 5860.f;
		y[0] = -8449.f;
		z[0] = 555.f;
		r[0] = 0.f;
		//Hole 1
		x[1] = 8424.f;
		y[1] = -3124.f;
		z[1] = 555.f;
		r[1] = 90.f;
		//Hole 2
		x[2] = 8424.f;
		y[2] = 2275.f;
		z[2] = 555.f;
		r[2] = 90.f;
		//Hole 3
		x[3] = 2714.f;
		y[3] = 8844.f;
		z[3] = 555.f;
		r[3] = 180.f;

		//MovePlayer();
	}
}

void AGameModeCPP::MovePlayer()
{

	FActorSpawnParameters spawnParams;
	FRotator rotator = FRotator(0, 0, r[CurrentHole]);
	FVector spawnLocation = FVector(x[CurrentHole], y[CurrentHole], z[CurrentHole]); //default to top left 

	GetWorld()->SpawnActor<AActor>(PlayerPawn, spawnLocation, rotator, spawnParams);

	CurrentHole++;
}
