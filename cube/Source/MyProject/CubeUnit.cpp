//-------------------------------------------------------------------
// ファイル		：CubeUnit.cpp
// 概要			：単体Cubeの処理
// 作成者		：19CU0236 林雲暉 
// 作成日		：2021/08/11		単体Cubeの基本構成制作
// 更新日		：2021/08/12		選択機能実装、マテリアル変えれる
//				：2021/08/15		選択機能の回転用ガイドラインの処理追加
//				：2021/08/23		選択機能のインプットイベントをStageCubeに移動した
//-------------------------------------------------------------------

#include "CubeUnit.h"
#include "StageCube_1.h"
#include "Engine.h"	

// コンストラクタ
ACubeUnit::ACubeUnit()
	: mCubeMesh(NULL)
	, mRootComponent(NULL)
	, mCubeMaterial_1(NULL)
	, mCubeMaterial_2(NULL)
	, mCubeMaterial_3(NULL)
	, mCubeMaterial_Instance(NULL)
	, mIsSelected(false)
	, serialNum(0)
	, mXCoordinate(0)
	, mYCoordinate(0)
	, mZCoordinate(0)
	, testInt(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MyRootSceneComponent"));
	RootComponent = mRootComponent;

	mCubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mCubeMesh"));

	mCubeMesh->SetupAttachment(RootComponent);

	mCubeMaterial_1 = CreateDefaultSubobject<UMaterial>(TEXT("CubeMaterial1"));

	mCubeMaterial_2 = CreateDefaultSubobject<UMaterial>(TEXT("CubeMaterial2"));


	//OnClicked.AddUniqueDynamic(this, &ACubeUnit::OnSelected);


	// マウスカーソルが重ねている時
	mCubeMesh->OnBeginCursorOver.AddUniqueDynamic(this, &ACubeUnit::OnOver2);
	mCubeMesh->OnEndCursorOver.AddUniqueDynamic(this, &ACubeUnit::EndOver2);
	//OnBeginCursorOver.AddUniqueDynamic(this, &ACubeUnit::OnOver);
	//OnEndCursorOver.AddUniqueDynamic(this, &ACubeUnit::EndOver);


	// Test Spawnning Diff Cube  21-08-25
	if (testInt == 1)
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("CubeUnit ConsTr")));


}

// 実行時に一度呼ばれる
void ACubeUnit::BeginPlay()
{
	Super::BeginPlay();

	if (testInt == 1)
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("CubeUnit BeginPlay")));


	APlayerController* myPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	 myPlayerController->bEnableMouseOverEvents = true;
	
	
	if (mCubeMesh != NULL)
	{
		// static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial(TEXT("/Game/Material/OutLine_V.OutLine_V'"));

		// mCubeMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

		if (mCubeMaterial_1 != NULL)
		{
			mCubeMesh->SetMaterial(0, mCubeMaterial_1);
		} // end if()
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("CubeMaterial Loading Failed")));
		} // end else


	} // end if()
	

} // void BeginPlay()


// 毎フレーム呼ばれる
void ACubeUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

} // void Tick()


void ACubeUnit::SetMeshAndMaterialOnBegin(UStaticMesh* newMesh, UMaterial* newMaterial_1, UMaterial* newMaterial_2, UMaterial* newMaterial_3)
{

	/*
	const ConstructorHelpers::FObjectFinder<UStaticMesh> WeaponA(TEXT("StaticMesh'/Game/Mesh/Autumn_Stage/Autumn_Cube_3.Autumn_Cube_3'"));

	// check if path is valid
	if (WeaponA.Succeeded())
	{
		// mesh = valid path
		mCubeMesh->SetStaticMesh(WeaponA.Object);
	}
	*/

	if (newMesh != NULL)
	{



		// mCubeMesh->SetStaticMesh(newMesh);

		if (newMaterial_1 != NULL && newMaterial_2 != NULL && newMaterial_3 != NULL)
		{
			mCubeMaterial_1 = newMaterial_1;
			mCubeMaterial_2 = newMaterial_2;
			mCubeMaterial_3 = newMaterial_3;

			mCubeMesh->SetMaterial(0, mCubeMaterial_1);
		} // end if()
		else GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("newMaterial is NULL!")));
		
		//else UE_LOG(LogTemp, Log, TEXT("newMaterial is NULL"));

	} // end if()
	else GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("newMesh is NULL!")));

	// else UE_LOG(LogTemp, Log, TEXT("newMesh is NULL"));

}  // void SetMeshAndMaterialOnBegin

void ACubeUnit::Test123(int x)
{
	 GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("-----  %d"), x));

}

void ACubeUnit::TestSet456(int y)
{
	testInt = y;
}

void ACubeUnit::OnSelected(AActor* Target, FKey ButtonPressed)
{

} // void OnSelected



void ACubeUnit::OnOver2(UPrimitiveComponent* Target)
{
	if (mCubeMesh != NULL)
	{
		if (mIsSelected == false)
		{
			if (mCubeMaterial_3 != NULL)
			{
				mCubeMesh->SetMaterial(0, mCubeMaterial_3);
			} // end if()
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("CubeMaterial Loading Failed")));
			} // end else			
		} // end if()
	} // end if()
} // void OnOver()


void ACubeUnit::EndOver2(UPrimitiveComponent* Target)
{
	if (mCubeMesh != NULL)
	{
		if (mIsSelected == false)
		{
			if (mCubeMaterial_1 != NULL)
			{
				mCubeMesh->SetMaterial(0, mCubeMaterial_1);
			} // end if()
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("CubeMaterial Loading Failed")));
			} // end else
		} // end if()
	} // end if()

} // void EndOver2



void ACubeUnit::ChangeMaterialFunc()
{
	if (mCubeMesh != NULL)
	{
		if (mIsSelected == false) {
			// 未選択の時

			if (mCubeMaterial_1 != NULL)
			{
				mCubeMesh->SetMaterial(0, mCubeMaterial_1);
			} // end if()
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("CubeMaterial_1 Fail Loading")));
			} // end else
		} // end if()
		else
		{
			// 選択しているの時

			if (mCubeMaterial_2 != NULL)
			{
				mCubeMesh->SetMaterial(0, mCubeMaterial_2);
			} // end if()
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("CubeMaterial_2 Fail Loading")));
			} // end else
		} // end else
	} // end if()

} // void ChangeMaterialFunc()


ACubeUnit::~ACubeUnit()
{
}

