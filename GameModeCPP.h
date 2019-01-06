// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameModeCPP.generated.h"


UCLASS()
class PROCEDURALGOLFV2_API AGameModeCPP : public AGameMode
{
	GENERATED_BODY()

protected:
	
	virtual void InitGameState();
	
	UPROPERTY(EditAnywhere)
		TSubclassOf<class APawn> PlayerPawn;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AController> PlayerController;
	
	AGameModeCPP();

	void MovePlayer();

	int CurrentHole; //current hole

	float x[8]; //x of spawn
	float y[8]; //y of spawn
	float z[8]; //z of spawn
	float r[8]; //yaw rotation of spawn

};
