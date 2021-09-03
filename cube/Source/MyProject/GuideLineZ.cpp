//-------------------------------------------------------------------
// ファイル		：GuideLineZ.cpp
// 概要			：回転用ガイドラインの処理
// 作成者		：19CU0236 林雲暉 
// 作成日		：2021/08/12		回転用ガイドラインの基本構成制作
// 更新日		：2021/08/13		選択機能実装、マテリアル変えれる
//				：2021/08/14		所属軸情報追加、表示非表示追加
//				：2021/08/23		選択機能のインプットイベントをStageCubeに移動した
//-------------------------------------------------------------------

#include "GuideLineZ.h"
#include "StageCube_1.h"
#include "Engine.h"	

// コンストラクタ
AGuideLineZ::AGuideLineZ()
	: mGuideLineMesh(NULL)
	, mRootComponent(NULL)
	, mLineMaterial_1(NULL)
	, mLineMaterial_2(NULL)
	, mLineMaterial_3(NULL)
	, mIsSelected(false)
	, mIsVisible(false)
	, mode(0)
	, mCubeMatInterface_1(NULL)
	, mCubeMatInterface_2(NULL)
	, mCubeMatInterface_3(NULL)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	mRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MyRootSceneComponent"));
	RootComponent = mRootComponent;

	mGuideLineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mGuideLineMesh"));

	mGuideLineMesh->SetupAttachment(RootComponent);

	mLineMaterial_1 = CreateDefaultSubobject<UMaterial>(TEXT("mLineMaterial_1"));
	mLineMaterial_2 = CreateDefaultSubobject<UMaterial>(TEXT("mLineMaterial_2"));

	mCubeMatInterface_1 = CreateDefaultSubobject<UMaterialInterface>(TEXT("CubeMatInterface1"));
	mCubeMatInterface_2 = CreateDefaultSubobject<UMaterialInterface>(TEXT("CubeMatInterface2"));
	mCubeMatInterface_3 = CreateDefaultSubobject<UMaterialInterface>(TEXT("CubeMatInterface3"));


	OnClicked.AddUniqueDynamic(this, &AGuideLineZ::OnSelected);
}

// 実行時に一度呼ばれる
void AGuideLineZ::BeginPlay()
{
	Super::BeginPlay();
	
	if (mGuideLineMesh != NULL)
	{
		if (mLineMaterial_1 != NULL)
		{
			mGuideLineMesh->SetMaterial(0, mLineMaterial_1);
		} // end if()
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("GuideLineMaterial_1 Loading Failed")));
		} // end else
	} // end if()

	mGuideLineMesh->SetVisibility(false);
	mGuideLineMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

} // void BeginPlay()

// 毎フレーム呼ばれる
void AGuideLineZ::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGuideLineZ::OnSelected(AActor* Target, FKey ButtonPressed)
{

	/*


	AActor* Owner1 = GetOwner();

	// 今は未選択の時
	if (mIsSelected == false)
	{
		mIsSelected = true;
		ChangeMaterialFunc();

		if (Owner1 != nullptr)
		{
			AStageCube_1* myStageCube = Cast< AStageCube_1>(Owner1);

			// 選択しているガイドライン解除
			if (myStageCube->mCurrentSelectedGuideLine != NULL)
			{
				myStageCube->mCurrentSelectedGuideLine->mIsSelected = false;
				myStageCube->mCurrentSelectedGuideLine->ChangeMaterialFunc();

				myStageCube->DetachFromGuideLine();
			} // end if()


			myStageCube->mCurrentSelectedGuideLine = this;

			// ガイドラインにアタッチ
			// mode 0 = XY-Plane(Z-axis),  1 = YZ-Plane(X-axis),  2 = XZ-Plane(Y-axis)
			myStageCube->AttachToGuideLine( mode );
			
			myStageCube->SetSelectingGuideLine(true);

		} // end if()

		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, FString::Printf(TEXT("%s   Line is Clicked"), *this->GetName()));

	} // end if()
	// 選択しているの時
	else
	{
		mIsSelected = false;
		ChangeMaterialFunc();

		if (Owner1 != nullptr)
		{
			AStageCube_1* myStageCube = Cast< AStageCube_1>(Owner1);

			myStageCube->DetachFromGuideLine();
			myStageCube->SetSelectingGuideLine(false);

			myStageCube->mCurrentSelectedGuideLine = NULL;

		} // end if()

		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, FString::Printf(TEXT("%s   Line is Unclicked"), *this->GetName()));

	} // end else


	*/

} // void OnSelected


void AGuideLineZ::ChangeMaterialFunc()
{
	if (mGuideLineMesh != NULL)
	{
		if (mIsSelected == false) {
			// 未選択の時

			if (mLineMaterial_1 != NULL)
			{
				mGuideLineMesh->SetMaterial(0, mLineMaterial_1);
			} // end if()
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("LineMaterial_1 Fail Loading")));
			} // end else
		} // end if()
		else
		{
			// 選択しているの時

			if (mLineMaterial_2 != NULL)
			{
				mGuideLineMesh->SetMaterial(0, mLineMaterial_2);
			} // end if()
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("LineMaterial_2 Fail Loading")));
			} // end else
		} // end else

	} // end if()

} // void ChangeMaterialFunc()


void AGuideLineZ::ChangeVisibilityFunc()
{
	if (mIsVisible == true)
	{
		mGuideLineMesh->SetVisibility(true);
		mGuideLineMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	} // end if()
	else
	{
		mGuideLineMesh->SetVisibility(false);
		mGuideLineMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	} // end else

} // void ChangeVisibilityFunc()


// デストラクタ
AGuideLineZ::~AGuideLineZ()
{
}
