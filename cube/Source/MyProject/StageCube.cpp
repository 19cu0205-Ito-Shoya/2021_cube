// Fill out your copyright notice in the Description page of Project Settings.


#include "StageCube.h"

// Sets default values
AStageCube::AStageCube()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStageCube::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStageCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AStageCube::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

