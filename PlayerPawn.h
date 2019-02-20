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

	//** Spring arm for positioning the camera above the ball */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	//	class USpringArmComponent* SpringArm;

	//** Camera to view the ball */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	//	class UCameraComponent* Camera;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;



public:
	
	//Tune Variables
	UPROPERTY(EditAnywhere, Category = AVariables)
		float defaultForce;

	UPROPERTY(EditAnywhere, Category = AVariables)
		float maxForce;

	UPROPERTY(EditAnywhere, Category = AVariables)
		float minForce;

	UPROPERTY(EditAnywhere, Category = AVariables)
		float force;

	//UPROPERTY(EditAnywhere, Category = AVariables)
	//	float forceMultiplier;

	UPROPERTY(EditAnywhere, Category = AVariables)
		float dampingDefault;

	UPROPERTY(EditAnywhere, Category = AVariables)
		float dampingMultiplier;

	UPROPERTY(EditAnywhere, Category = AVariables)
		float JumpImpulse;

	UPROPERTY(EditAnywhere, Category = Camera)
		float BaseTurnRate;

	UPROPERTY(EditAnywhere, Category = Camera)
		float BaseLookUpRate;

	UPROPERTY(EditAnywhere, Category = Camera)
		float BaseZoomRate;

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
	//FORCEINLINE class USpringArmComponent* GetSpringArm() const { return SpringArm; }
	/** Returns Camera subobject **/
	//FORCEINLINE class UCameraComponent* GetCamera() const { return Camera; }
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

	void UpdateHUDStrings();
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly)
		FText ForceDisplayString;

	//void RotateCameraCW();
	//void RotateCameraCCW();

	//void RotateCameraCWRelease();
	//void RotateCameraCCWRelease();

	void CameraZoomIn();
	void CameraZoomOut();

	void CameraZoomInRelease();
	void CameraZoomOutRelease();


	void Shoot();

	FRotator shootDirection;



	//Non Tune Variables
	bool canShoot, canSetShoot, touchedFlag;

	int rotating, slowValue; //velocities below this are considered slow
	//int cameraRotating;
	int cameraZooming;

	float incrementForce, realDamping, iceDamping, sandDamping;

	int overlappingIce;
	int overlappingSand;
	int CurrentHole;
	float sensitivity;
	FTimerHandle canSetShootTimer;
	FTimerHandle flagTimer;

	void canSetShootMethod();
	void RespawnPlayer();
	int shotsTaken = 0;
	bool slowMoving;

	UFUNCTION()
		virtual void OnOverlap(UPrimitiveComponent* HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	//FVector Dimensions = FVector(300, 0, 0);
	//FVector AxisVector = FVector(0, 0, 1);
	//float Multiplier = 50.f;
	//float AngleAxis = 0.f;

	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	void ZoomInRate(float Rate);

	bool LeftClickPressed;
	bool RightClickPressed;

	void LeftClick();
	void LeftRelease();

	void RightClick();
	void RightRelease();

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FVector NextFlag;

	void SetNextFlag(FVector location);
	void RestartPressed();
	void ResetGamePressed();
	bool IsItNewLevel = false;

	bool DrawDebugText = true;
	bool DrawFlagHitText = false;

	FVector LastSafeLocation;
	bool RainbowLines = true;

	float customForceMulti;
	float killZ;
}
;