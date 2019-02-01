// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuWidget.h"

void UMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Button01->OnClicked.AddDynamic(this, &UMenuWidget::OnClick);
}

void UMenuWidget::OnClick()
{
	UE_LOG(LogTemp, Warning, TEXT("The hardest button to button"));
}