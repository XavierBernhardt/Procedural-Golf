// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlagActor.generated.h"

UCLASS()
class PROCEDURALGOLFV2_API AFlagActor : public AActor
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere)
		USceneComponent * SceneRoot;
public:	
	// Sets default values for this actor's properties
	AFlagActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		virtual void OnOverlap(UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);


	
};
