// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Components/Button.h"
#include "MenuWidget.generated.h"

UCLASS(ABSTRACT)
class PROCEDURALGOLFV2_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

		void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
		UButton* Button01;


	UFUNCTION()
		void OnClick();
};
