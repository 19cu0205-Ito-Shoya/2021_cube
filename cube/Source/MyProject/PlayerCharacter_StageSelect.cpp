// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter_StageSelect.h"

// Sets default values
APlayerCharacter_StageSelect::APlayerCharacter_StageSelect()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerCharacter_StageSelect::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter_StageSelect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter_StageSelect::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

