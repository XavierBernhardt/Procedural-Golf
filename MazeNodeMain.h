// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MazeNodeMain.generated.h"

UCLASS()
class PROCEDURALGOLFV2_API AMazeNodeMain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMazeNodeMain();
	int type;
	int floor;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, Category = Maze1)
		TSubclassOf<class AActor> MazeN;

	UPROPERTY(EditAnywhere, Category = Maze1)
		TSubclassOf<class AActor> MazeC;

	UPROPERTY(EditAnywhere, Category = Maze1)
		TSubclassOf<class AActor> MazeL;

	UPROPERTY(EditAnywhere, Category = Maze1)
		TSubclassOf<class AActor> MazeI;

	UPROPERTY(EditAnywhere, Category = Maze1)
		TSubclassOf<class AActor> MazeT;

	UPROPERTY(EditAnywhere, Category = Maze1)
		TSubclassOf<class AActor> MazeX;

	UPROPERTY(EditAnywhere, Category = Maze1)
		TSubclassOf<class AActor> MazeFloor;
};
