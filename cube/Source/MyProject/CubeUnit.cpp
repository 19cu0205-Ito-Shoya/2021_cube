//-------------------------------------------------------------------
// ファイル		：CubeUnit.cpp
// 概要			：単体Cubeの処理
// 作成者		：19CU0236 林雲暉 
// 作成日		：2021/08/11		単体Cubeの基本構成制作
// 更新日		：2021/08/12		選択機能実装、マテリアル変えれる
//				：2021/08/15		選択機能の回転用ガイドラインの処理追加
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
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MyRootSceneComponent"));
	RootComponent = mRootComponent;

	mCubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mCubeMesh"));

	mCubeMesh->SetupAttachment(RootComponent);

	mCubeMaterial_1 = CreateDefaultSubobject<UMaterial>(TEXT("CubeMaterial1"));

	mCubeMaterial_2 = CreateDefaultSubobject<UMaterial>(TEXT("CubeMaterial2"));


	OnClicked.AddUniqueDynamic(this, &ACubeUnit::OnSelected);

}

// 実行時に一度呼ばれる
void ACubeUnit::BeginPlay()
{
	Super::BeginPlay();
	

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


void ACubeUnit::OnSelected(AActor* Target, FKey ButtonPressed)
{
	AActor* Owner1 = GetOwner();

	// このCube今は未選択の時
	if (mIsSelected == false)
	{
		mIsSelected = true;
		ChangeMaterialFunc();

		if (Owner1 != nullptr)
		{
			AStageCube_1* myStageCube = Cast< AStageCube_1>(Owner1);

			// 既にCubeが選択している時
			if (myStageCube->mCurrentSelectedCube != NULL)
			{
				myStageCube->mCurrentSelectedCube->mIsSelected = false;
				myStageCube->mCurrentSelectedCube->ChangeMaterialFunc();

				if (myStageCube->mCurrentSelectedGuideLine != NULL)
				{
					myStageCube->DetachFromGuideLine();
					myStageCube->SetSelectingGuideLine(false);
					myStageCube->mCurrentSelectedGuideLine->mIsSelected = false;
					myStageCube->mCurrentSelectedGuideLine->ChangeMaterialFunc();
					myStageCube->mCurrentSelectedGuideLine = NULL;
				} // end if()

			} // end if()
			// Cubeが選択していない時
			else myStageCube->ChangeAllGuideLinesVisibility(true);
			

			myStageCube->mCurrentSelectedCube = this;
			myStageCube->SetGuideLinePosition();
			myStageCube->SetSelectingCube(true);

		} // end if()
		
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Magenta, FString::Printf(TEXT("%s   Unit is Clicked~!"), *this->GetName()));

	} // end if()
	// このCube今は選択しているの時
	else
	{
		mIsSelected = false;
		ChangeMaterialFunc();

		if (Owner1 != nullptr)
		{
			AStageCube_1* myStageCube = Cast< AStageCube_1>(Owner1);

			if (myStageCube->mCurrentSelectedGuideLine != NULL)
			{
				myStageCube->DetachFromGuideLine();
				myStageCube->SetSelectingGuideLine(false);
				myStageCube->mCurrentSelectedGuideLine->mIsSelected = false;
				myStageCube->mCurrentSelectedGuideLine->ChangeMaterialFunc();
				myStageCube->mCurrentSelectedGuideLine = NULL;
			} // end if()

			myStageCube->mCurrentSelectedCube = NULL;
			myStageCube->SetSelectingCube(false);
			myStageCube->ChangeAllGuideLinesVisibility(false);
		} // end if()

		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Magenta, FString::Printf(TEXT("%s   Unit is Unclicked~!"), *this->GetName()));

	} // end else

} // void OnSelected

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

