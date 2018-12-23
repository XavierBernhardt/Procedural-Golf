// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

UCLASS()
class PROCEDURALGOLFV2_API APlayerPawn : public APawn
{
	GENERATED_BODY()

	/** StaticMesh used for the ball */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Ball;

	/** StaticMesh used for the Cyl */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	//	class UStaticMeshComponent* Cyl;

	/** Spring arm for positioning the camera above the ball */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* SpringArm;

	/** Camera to view the ball */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;


public:
	// Sets default values for this pawn's properties
	APlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Returns Ball subobject **/
	FORCEINLINE class UStaticMeshComponent* GetBall() const { return Ball; }
	/** Returns SpringArm subobject **/
	FORCEINLINE class USpringArmComponent* GetSpringArm() const { return SpringArm; }
	/** Returns Camera subobject **/
	FORCEINLINE class UCameraComponent* GetCamera() const { return Camera; }
	/** Returns Ball subobject **/
	//FORCEINLINE class UStaticMeshComponent* getCyl() const { return Cyl; }

	//Keyboard Functions
	void RotateCW();
	void RotateCCW();

	void RotateCWRelease();
	void RotateCCWRelease();

	void ForceAdd();
	void ForceRemove();
	
	void ForceAddRelease();
	void ForceRemoveRelease();



	void Shoot();

	FRotator shootDirection;

	//Tune Variables
	UPROPERTY(EditAnywhere, Category = AVariables)
	float defaultForce;

	UPROPERTY(EditAnywhere, Category = AVariables)
	float maxForce;

	UPROPERTY(EditAnywhere, Category = AVariables)
	float minForce;

	UPROPERTY(EditAnywhere, Category = AVariables)
	float force;

	//Non Tune Variables
	bool canShoot, canSetShoot;

	int rotating;
	float incrementForce;

	FTimerHandle canSetShootTimer;
	void canSetShootMethod();

};
