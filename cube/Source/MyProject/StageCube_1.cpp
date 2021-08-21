//---------------------------------------------------------------------------------
// ファイル		：StageCube_1.cpp
// 概要			：総体Cubeの処理
// 作成者		：19CU0236 林雲暉 
// 作成日		：2021/08/11		総体Cubeの基本構成制作
// 更新日		：2021/08/12		単体Cube生成と配列入り、選択機能
//				：2021/08/13		回転のため配列によるMatrix計算
//				：2021/08/13		回転用ガイドラインの生成、選択機能、アタッチ機能
//				：2021/08/14		回転用ガイドラインの軸情報追加、デタッチ機能
//				：2021/08/16		単体Cubeを選択した時に回転用ガイドラインの処理(配置、可視性)
//				：2021/08/17		左回転90度(=右回転270度)と回転180度追加
//				：2021/08/18		総体Cubeの回転操作追加、ガイドラインの回転操作追加
//				：2021/08/19		Camera回転の矯正、ガイドライン回転の矯正
//				：2021/08/20		回転の修正、回転計算方法はQuaternionから計算に変更
//				：2021/08/21		ガイドラインYの修正
//---------------------------------------------------------------------------------

#include "StageCube_1.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine.h"

// コンストラクタ
AStageCube_1::AStageCube_1()
	: mRootComponent(NULL)
	, mCubesRootComponent(NULL)
	, mSpringArm(NULL)
	, mCamera(NULL)
	, mCurrentSelectedCube(NULL)
	, mCurrentSelectedGuideLine(NULL)
	, mGuideLineXaxis(NULL)
	, mGuideLineYaxis(NULL)
	, mGuideLineZaxis(NULL)
	, isSelectingCube(false)
	, isSelectingGuideLine(false)
	, isDraggingGuideLine(false)
	, isMovingCamera(false)
	, mCameraTurnScaleX(3.f)
	, mCameraTurnScaleY(3.f)
	, mMouseXvalue(0.f)
	, mMouseYvalue(0.f)
	, guideLineTurnningScale(5.f)
	, mStartRotateDegree(0.f,0.f,0.f)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MyRootSceneComponent"));
	RootComponent = mRootComponent;

	mCubesRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MyCubesRootSceneComponent"));
	mCubesRootComponent->SetupAttachment(RootComponent);

	// スプリングアームのオブジェクトを生成
	mSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("mSpringArm"));

	// スプリングアームにRootComponentをアタッチ
	mSpringArm->SetupAttachment(RootComponent);

	// カメラとプレイヤーの距離
	mSpringArm->TargetArmLength = 800.f;

	// カメラの子リジョンテストを行うかを設定
	mSpringArm->bDoCollisionTest = false;

	// カメラを生成
	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("mCamera"));

	// カメラにスプリングアームをアタッチ
	mCamera->SetupAttachment(mSpringArm, USpringArmComponent::SocketName);


	OnClicked.AddUniqueDynamic(this, &AStageCube_1::OnSelected);

}

// デストラクタ
AStageCube_1::~AStageCube_1()
{
}

// 実行時に一度呼ばれる
void AStageCube_1::BeginPlay()
{
	Super::BeginPlay();
	
	mSpringArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));

	// FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);

	// ================================ Cube の生成  ================================

	bp_CubeUnit = TSoftClassPtr<AActor>(FSoftObjectPath(*path)).LoadSynchronous();	// pathにあるBPを取得

	if (bp_CubeUnit != nullptr)
	{
		int counter = 1;							// 計数器
		int remainder = 0;							// 余り

		// 原点の位置
		float x1 = 100.f;
		float y1 = -100.f;
		float z1 = 100.f;

		// 配列用の座標位置
		int xC = 0;		// 座標X
		int yC = 0;		// 座標Y
		int zC = 0;		// 座標Z

		ACubeUnit* cubeGen;								// Cube Actor生成用

		TArray< TArray<ACubeUnit*>> tempArray2D;		// 仮保存用2D Array
		TArray<ACubeUnit*> tempArray1D;					// 仮保存用Array 

		// *** For test position 8/12
		int tempSerialNum = 1000;

		for (int i = 0; i <= 26; ++i, ++counter, ++yC) {

			cubeGen = GetWorld()->SpawnActor<ACubeUnit>(bp_CubeUnit);				// スポーンCube Actor

			// 配列の位置計算
			remainder = counter % 3;
			if (remainder == 1)
				y1 = -100.f;
			else if (remainder == 2)
				y1 = 0.f;
			else if (remainder == 0)
				y1 = 100.f;

			if (cubeGen != NULL)
			{
				cubeGen->mRootComponent->AttachToComponent(mCubesRootComponent, AttachRules);
				cubeGen->SetActorRelativeLocation(FVector(x1, y1, z1));
				cubeGen->SetOwner(this);

				// *** For test position 8/12
				cubeGen->serialNum = tempSerialNum;
				cubeGen->mXCoordinate = xC;
				cubeGen->mYCoordinate = yC;
				cubeGen->mZCoordinate = zC;

				tempArray1D.Add(cubeGen);

			} // end if()

			// ===================  Debug用 + Editorで見える  =======================
				if (counter < 10)
				{
					d1Cube.Add(cubeGen);
				} // end if()
				else if (counter >= 10 && counter < 19)
				{
					d2Cube.Add(cubeGen);
				} // end if()
				else
				{
					d3Cube.Add(cubeGen);
				} // end else
			// ======================================================================

			// 次の行
			if (counter % 3 == 0 )
			{
				tempArray2D.Add(tempArray1D);
				tempArray1D.Empty();

				x1 -= 100.f;

				++xC;
				yC = -1;		// forは＋1にする

				// *** For test position 8/12
				tempSerialNum += 7;
			}  // end if()
			// 次の階層
			if (counter % 9 == 0)
			{
				CubeArray3D.Add(tempArray2D);
				tempArray2D.Empty();

				z1 -= 100.f;
				x1 = 100.f;
				y1 = -100.f;

				xC = 0;
				yC = -1;		// forは＋1にする
				++zC;

				// *** For test position 8/12
				tempSerialNum += 70;
			} // end if()

			// *** For test position 8/12
			++tempSerialNum;
		} // end for()

		// test
		// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("===== %s"), *CubeArray3D[0][0][2]->GetName()));
		// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("===== %s"), *CubeArray3D[1][0][2]->GetName()));

	} // end if()


	// ==========================  Guide Line の生成  ==========================

	FString pathGL = "Blueprint'/Game/BP/BP_GuideLineZ.BP_GuideLineZ_C'";
	TSubclassOf<class AActor> bp_GuideLineZ = TSoftClassPtr<AActor>(FSoftObjectPath(*pathGL)).LoadSynchronous();	// pathにあるBPを取得

	// Z Axis
	if (bp_GuideLineZ != nullptr)
	{
		mGuideLineZaxis = GetWorld()->SpawnActor<AGuideLineZ>(bp_GuideLineZ);

		if (mGuideLineZaxis != NULL)
		{
			mGuideLineZaxis->AttachToComponent(mCubesRootComponent, AttachRules);
			mGuideLineZaxis->SetActorRelativeLocation(FVector(0.f, 0.f, 0.f));
			mGuideLineZaxis->SetActorScale3D(FVector(50.f, 30.f, 50.f));
			mGuideLineZaxis->mGuideLineMesh->SetRelativeRotation(FRotator(0.f, 0.f, 90.f));
			mGuideLineZaxis->SetOwner(this);

			// mode 0 = XY-Plane(Z-axis),  1 = YZ-Plane(X-axis),  2 = XZ-Plane(Y-axis)
			mGuideLineZaxis->mode = 0;
		} // end if()

	} // end if()

	// X Axis
	if (bp_GuideLineZ != nullptr)
	{
		mGuideLineXaxis = GetWorld()->SpawnActor<AGuideLineZ>(bp_GuideLineZ);

		if (mGuideLineXaxis != NULL)
		{
			mGuideLineXaxis->AttachToComponent(mCubesRootComponent, AttachRules);
			mGuideLineXaxis->SetActorRelativeLocation(FVector(0.f, 0.f, 0.f));
			mGuideLineXaxis->SetActorScale3D(FVector(50.f, 30.f, 50.f));
			mGuideLineXaxis->mGuideLineMesh->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
			mGuideLineXaxis->SetOwner(this);

			mGuideLineXaxis->mode = 1;
		} // end if()
	} // end if()

	// Y Axis
	// =================  Caution  8/21  ================
	// ガイドライン Y のActorのRotation設定は他のガイドラインと違います、要注意です。
	// (元々はPitchを回転すると、オイラー角の制限があった為に、こうになった)
	if (bp_GuideLineZ != nullptr)
	{
		mGuideLineYaxis = GetWorld()->SpawnActor<AGuideLineZ>(bp_GuideLineZ);

		if (mGuideLineYaxis != NULL)
		{
			mGuideLineYaxis->AttachToComponent(mCubesRootComponent, AttachRules);
			mGuideLineYaxis->SetActorRelativeLocation(FVector(0.f, 0.f, 0.f));
			mGuideLineYaxis->SetActorScale3D(FVector(50.f, 30.f, 50.f));
			mGuideLineYaxis->mGuideLineMesh->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
			mGuideLineYaxis->SetActorRelativeRotation(FRotator(0.f, 90.f, 0.f));
			mGuideLineYaxis->SetOwner(this);

			mGuideLineYaxis->mode = 2;
		} // end if()
	} // end if()

	// マウスのクリックイベント用
	APlayerController* myPlayerController =	UGameplayStatics::GetPlayerController(GetWorld(), 0);

	myPlayerController->bShowMouseCursor = true;
	myPlayerController->bEnableMouseOverEvents = true;
	myPlayerController->bEnableClickEvents = true;
	// for test
	// RoatateTheCubesRight90( 1);
	// Replace3Array();
	// RoatateTheCubesLeft90(0);


} // BeginPlay()


// 毎フレーム呼ばれる
void AStageCube_1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

} // Tick

// 各入力関係メソッドとのバインド処理
void AStageCube_1::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	// enable 'Show Mouse Cursor', you stop sending input events to the actors and return focus to the user.
	// It works only when you click(press) on mouse button
	InputComponent->BindAxis("MoveMouseX", this, &AStageCube_1::MoveMouseX);
	InputComponent->BindAxis("MoveMouseY", this, &AStageCube_1::MoveMouseY);

	InputComponent->BindAction("MouseLeftClick", IE_Pressed, this, &AStageCube_1::MouseLeftButtonPressed);
	InputComponent->BindAction("MouseLeftClick", IE_Released, this, &AStageCube_1::MouseLeftButtonReleased);
	 
	// MoveMouseXY
	// MouseLeftClick
	// MouseLeftButtonPressed

} // SetupPlayerInputComponent


// **NOTE 8/13, 8/15 fixed Y axis**
// 右回転90度、軸の回転方向要確認
// mode 0 = XY-Plane(Z-axis),  1 = YZ-Plane(X-axis),  2 = XZ-Plane(Y-axis)
// 3D Array [Z][X][Y]
//         Dep Row Col
// 
// 180 degree need another function
void AStageCube_1::RoatateTheCubesRight90( int mode)
{
	int totalCol = 3;

	if (mode == 0)
	{
		// 8/13)) 選択されたcubeの位置取得
		int tempZ = 0;
		if (mCurrentSelectedCube != NULL)
			tempZ = mCurrentSelectedCube->mZCoordinate;
		else {
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("RoatateTheCubesRight90 : mCurrentSelectedCube is NULL")));
			return;
		} // end else

		// 主対角線転置行列 Transposed Matrix
		for (int i = 0; i < totalCol; ++i) {
			for (int j = 0; j < i; ++j) {
				Swap(CubeArray3D[tempZ][i][j], CubeArray3D[tempZ][j][i]);
				SwapCoordinate(CubeArray3D[tempZ][i][j], CubeArray3D[tempZ][j][i]);
			} // end for()
		} // end for()

		// 左右垂直線交換
		for (int i = 0; i < totalCol / 2; ++i) {
			for (int j = 0; j < totalCol; ++j) {
				Swap(CubeArray3D[tempZ][j][i], CubeArray3D[tempZ][j][totalCol - i - 1]);
				SwapCoordinate(CubeArray3D[tempZ][j][i], CubeArray3D[tempZ][j][totalCol - i - 1]);
			} // end for()
		} // end for()

	} // end if()
	else if (mode == 1)
	{
		// 選択されたcubeの位置取得
		int tempX = 0;
		if (mCurrentSelectedCube != NULL)
			tempX = mCurrentSelectedCube->mXCoordinate;
		else {
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("RoatateTheCubesRight90 : mCurrentSelectedCube is NULL")));
			return;
		} // end else

		// 主対角線転置行列 Transposed Matrix
		for (int i = 0; i < totalCol; ++i) {
			for (int j = 0; j < i; ++j) {
				Swap(CubeArray3D[i][tempX][j], CubeArray3D[j][tempX][i]);
				SwapCoordinate(CubeArray3D[i][tempX][j], CubeArray3D[j][tempX][i]);
			} // end for()
		} // end for()

		// 左右垂直線交換
		for (int i = 0; i < totalCol / 2; ++i) {
			for (int j = 0; j < totalCol; ++j) {
				Swap(CubeArray3D[j][tempX][i], CubeArray3D[j][tempX][totalCol - i - 1]);
				SwapCoordinate(CubeArray3D[j][tempX][i], CubeArray3D[j][tempX][totalCol - i - 1]);
			} // end for()
		} // end for()

	} // end if()
	else if (mode == 2)
	{
		// 選択されたcubeの位置取得
		int tempY = 0;
		if (mCurrentSelectedCube != NULL)
			tempY = mCurrentSelectedCube->mYCoordinate;
		else {
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("RoatateTheCubesRight90 : mCurrentSelectedCube is NULL")));
			return;
		} // end else

		// 主対角線転置行列 Transposed Matrix
		for (int i = 0; i < totalCol; ++i) {
			for (int j = 0; j < i; ++j) {
				Swap(CubeArray3D[i][j][tempY], CubeArray3D[j][i][tempY]);
				SwapCoordinate(CubeArray3D[i][j][tempY], CubeArray3D[j][i][tempY]);
			} // end for()
		} // end for()

		// 左右垂直線交換
		for (int i = 0; i < totalCol / 2; ++i) {
			for (int j = 0; j < totalCol; ++j) {
				Swap(CubeArray3D[j][i][tempY], CubeArray3D[j][totalCol - i - 1][tempY]);
				SwapCoordinate(CubeArray3D[j][i][tempY], CubeArray3D[j][totalCol - i - 1][tempY]);
			} // end for()
		} // end for()

	} // end if()
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("RoatateTheCubesRight90's mode error!")));
	} // end else

	// Debug用　editorで確認用
	Replace3Array();

} // void RoatateTheCubesRight90()


/*
==================================  検証用  ==================================
// mode1 sample
if test, need to delete else return;


		for (int i = 0; i < totalCol; ++i) {
			for (int j = 0; j < totalCol; ++j)
			{
				ACubeUnit* myCubeUnit = Cast< ACubeUnit>(CubeArray3D[i][tempX][j]);
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Bef  %d  "), myCubeUnit->serialNum));
			} // end for()
		} // end for()


//========================================================


		for (int i = 0; i < totalCol; ++i) {
			for (int j = 0; j < totalCol; ++j)
			{
				ACubeUnit* myCubeUnit = Cast< ACubeUnit>(CubeArray3D[i][tempX][j]);
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("after  %d  "), myCubeUnit->serialNum));
			} // end for()
		} // end for()


==================================  検証用  ==================================
*/


// 左回転90度(=右回転270度)
// mode 0 = XY-Plane(Z-axis),  1 = YZ-Plane(X-axis),  2 = XZ-Plane(Y-axis)
// 3D Array [Z][X][Y]
//         Dep Row Col
void AStageCube_1::RoatateTheCubesLeft90( const int mode)
{
	// CubeArray3D
	int totalCol = 3;

	if (mode == 0)
	{
		// 選択されたcubeの位置取得
		int tempZ = 0;
		if (mCurrentSelectedCube != NULL)
			tempZ = mCurrentSelectedCube->mZCoordinate;
		else {
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("RoatateTheCubesLeft90 : mCurrentSelectedCube is NULL")));
			return;
		} // end else

		// 主対角線転置行列 Transposed Matrix
		for (int i = 0; i < totalCol; ++i) {
			for (int j = 0; j < i; ++j) {
				Swap(CubeArray3D[tempZ][i][j], CubeArray3D[tempZ][j][i]);
				SwapCoordinate(CubeArray3D[tempZ][i][j], CubeArray3D[tempZ][j][i]);
			} // end for()
		} // end for()

		// 上下水平交換
		for (int i = 0; i < totalCol / 2; ++i) {
			for (int j = 0; j < totalCol; ++j) {
				Swap(CubeArray3D[tempZ][i][j], CubeArray3D[tempZ][totalCol - i - 1][j]);
				SwapCoordinate(CubeArray3D[tempZ][i][j], CubeArray3D[tempZ][totalCol - i - 1][j]);
			} // end for()
		} // end for()

	} // end if()
	else if (mode == 1)
	{
		// 選択されたcubeの位置取得
		int tempX = 0;
		if (mCurrentSelectedCube != NULL)
			tempX = mCurrentSelectedCube->mXCoordinate;
		else {
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("RoatateTheCubesLeft90 : mCurrentSelectedCube is NULL")));
			return;
		} // end else
		
		// 主対角線転置行列 Transposed Matrix
		for (int i = 0; i < totalCol; ++i) {
			for (int j = 0; j < i; ++j) {
				Swap(CubeArray3D[i][tempX][j], CubeArray3D[j][tempX][i]);
				SwapCoordinate(CubeArray3D[i][tempX][j], CubeArray3D[j][tempX][i]);
			} // end for()
		} // end for()

		// 上下水平交換
		for (int i = 0; i < totalCol / 2; ++i) {
			for (int j = 0; j < totalCol; ++j) {
				Swap(CubeArray3D[i][tempX][j], CubeArray3D[totalCol - i - 1][tempX][j]);
				SwapCoordinate(CubeArray3D[i][tempX][j], CubeArray3D[totalCol - i - 1][tempX][j]);
			} // end for()
		} // end for()

	} // end if()
	else if (mode == 2)
	{
		// 選択されたcubeの位置取得
		int tempY = 0;
		if (mCurrentSelectedCube != NULL)
			tempY = mCurrentSelectedCube->mYCoordinate;
		else {
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("RoatateTheCubesLeft90 : mCurrentSelectedCube is NULL")));
			return;
		} // end else

		// 主対角線転置行列 Transposed Matrix
		for (int i = 0; i < totalCol; ++i) {
			for (int j = 0; j < i; ++j) {
				Swap(CubeArray3D[i][j][tempY], CubeArray3D[j][i][tempY]);
				SwapCoordinate(CubeArray3D[i][j][tempY], CubeArray3D[j][i][tempY]);
			} // end for()
		} // end for()

		// 上下水平交換
		for (int i = 0; i < totalCol / 2; ++i) {
			for (int j = 0; j < totalCol; ++j) {
				Swap(CubeArray3D[i][j][tempY], CubeArray3D[totalCol - i - 1][j][tempY]);
				SwapCoordinate(CubeArray3D[i][j][tempY], CubeArray3D[totalCol - i - 1][j][tempY]);
			} // end for()
		} // end for()

	} // end if()
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("RoatateTheCubesLeft90's mode error!")));
	} // end else

	// Debug用　editorで確認用
	Replace3Array();

} // void RoatateTheCubesLeft90()


// 回転180度
// mode 0 = XY-Plane(Z-axis),  1 = YZ-Plane(X-axis),  2 = XZ-Plane(Y-axis)
// 3D Array [Z][X][Y]
//         Dep Row Col
// 
void AStageCube_1::RoatateTheCubes180( const int mode)
{
	// CubeArray3D
	int totalCol = 3;

	if (mode == 0)
	{

		int tempZ = 0;
		if (mCurrentSelectedCube != NULL)
			tempZ = mCurrentSelectedCube->mZCoordinate;
		else {
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("RoatateTheCubesLeft90 : mCurrentSelectedCube is NULL")));
			return;
		} // end else

		// 上下水平交換
		for (int i = 0; i < totalCol / 2; ++i) {
			for (int j = 0; j < totalCol; ++j) {
				Swap(CubeArray3D[tempZ][i][j], CubeArray3D[tempZ][totalCol - i - 1][j]);
				SwapCoordinate(CubeArray3D[tempZ][i][j], CubeArray3D[tempZ][totalCol - i - 1][j]);
			} // end for()
		} // end for()

		// 左右垂直線交換
		for (int i = 0; i < totalCol / 2; ++i) {
			for (int j = 0; j < totalCol; ++j) {
				Swap(CubeArray3D[tempZ][j][i], CubeArray3D[tempZ][j][totalCol - i - 1]);
				SwapCoordinate(CubeArray3D[tempZ][j][i], CubeArray3D[tempZ][j][totalCol - i - 1]);
			} // end for()
		} // end for()

	} // end if()
	else if (mode == 1)
	{
		// 選択されたcubeの位置取得
		int tempX = 0;
		if (mCurrentSelectedCube != NULL)
			tempX = mCurrentSelectedCube->mXCoordinate;
		else {
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("RoatateTheCubesLeft90 : mCurrentSelectedCube is NULL")));
			return;
		} // end else

		// 上下水平交換
		for (int i = 0; i < totalCol / 2; ++i) {
			for (int j = 0; j < totalCol; ++j) {
				Swap(CubeArray3D[i][tempX][j], CubeArray3D[totalCol - i - 1][tempX][j]);
				SwapCoordinate(CubeArray3D[i][tempX][j], CubeArray3D[totalCol - i - 1][tempX][j]);
			} // end for()
		} // end for()

		// 左右垂直線交換
		for (int i = 0; i < totalCol / 2; ++i) {
			for (int j = 0; j < totalCol; ++j) {
				Swap(CubeArray3D[j][tempX][i], CubeArray3D[j][tempX][totalCol - i - 1]);
				SwapCoordinate(CubeArray3D[j][tempX][i], CubeArray3D[j][tempX][totalCol - i - 1]);
			} // end for()
		} // end for()

	} // end if()
	else if (mode == 2)
	{
		// 選択されたcubeの位置取得
		int tempY = 0;
		if (mCurrentSelectedCube != NULL)
			tempY = mCurrentSelectedCube->mYCoordinate;
		else {
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("RoatateTheCubesLeft90 : mCurrentSelectedCube is NULL")));
			return;
		} // end else

		// 上下水平交換
		for (int i = 0; i < totalCol / 2; ++i) {
			for (int j = 0; j < totalCol; ++j) {
				Swap(CubeArray3D[i][j][tempY], CubeArray3D[totalCol - i - 1][j][tempY]);
				SwapCoordinate(CubeArray3D[i][j][tempY], CubeArray3D[totalCol - i - 1][j][tempY]);
			} // end for()
		} // end for()

		// 左右垂直線交換
		for (int i = 0; i < totalCol / 2; ++i) {
			for (int j = 0; j < totalCol; ++j) {
				Swap(CubeArray3D[j][i][tempY], CubeArray3D[j][totalCol - i - 1][tempY]);
				SwapCoordinate(CubeArray3D[j][i][tempY], CubeArray3D[j][totalCol - i - 1][tempY]);
			} // end for()
		} // end for()

	} // end if()
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("RoatateTheCubesLeft90's mode error!")));
	} // end else

	// Debug用　editorで確認用
	Replace3Array();

} // void RoatateTheCubes180()

void AStageCube_1::MoveMouseX(const float _axisValue)
{

	// GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Emerald, FString::SanitizeFloat(_axisValue));

	if (_axisValue != 0.f)
	{
		if (isDraggingGuideLine && mCurrentSelectedGuideLine != NULL)
		{
			if (mCurrentSelectedGuideLine == mGuideLineZaxis)
			{
				// wiil be strange when turn on other side
				mCurrentSelectedGuideLine->mRootComponent->SetRelativeRotation(MyCombineRotators(mCurrentSelectedGuideLine->mRootComponent->GetRelativeRotation(), FRotator(0.f, -_axisValue * guideLineTurnningScale, 0.f)));
			} // end if
			else if (mCurrentSelectedGuideLine == mGuideLineXaxis)
			{
				// wiil be strange when turn on other side
				mCurrentSelectedGuideLine->mRootComponent->SetRelativeRotation(MyCombineRotators(mCurrentSelectedGuideLine->mRootComponent->GetRelativeRotation(), FRotator(0.f, 0.f, _axisValue * guideLineTurnningScale)));

			} // end if
			else if (mCurrentSelectedGuideLine == mGuideLineYaxis)
			{
				// =================  Caution  8/21  ================
				// ガイドライン Y のActorのRotation設定は他のガイドラインと違います、要注意です。
				// (元々はPitchを回転すると、オイラー角の制限があった為に、こうになった)
				// wiil be strange when turn on other side
				mCurrentSelectedGuideLine->mRootComponent->SetRelativeRotation(MyCombineRotators(mCurrentSelectedGuideLine->mRootComponent->GetRelativeRotation(), FRotator(0.f, 0.f, -_axisValue * guideLineTurnningScale)));
			} // end if

		} // end if
		else if (isMovingCamera)
		{
			// will affect roll, don't use
			// mSpringArm->AddRelativeRotation(FRotator(0.f, _axisValue * mCameraTurnScaleX, 0.f));

			// Cameraを回転
			// MyCombineRotatorsを使って調整する
			FRotator tempR = MyCombineRotators(mSpringArm->GetRelativeRotation(), FRotator(0.f, _axisValue * mCameraTurnScaleX, 0.f));
			tempR.Roll = 0.f;
		
			mCameraTurnScaleX = AdjustCameraTurnScale(tempR.Pitch);

			// Pitchが90度ならないように(オイラー角の制限)
			if (tempR.Pitch > 85.f)
				tempR.Pitch = 85.f;
			else if (tempR.Pitch < -85.f)
				tempR.Pitch = -85.f;

			mSpringArm->SetRelativeRotation(tempR);

			// Cubeを回転
			// mCubesRootComponent->AddRelativeRotation(FRotator(0.f, -_axisValue * mCameraTurnScaleX, 0.f));

			// will affect roll, don't use
			// mCubesRootComponent->AddLocalRotation(FRotator(0.f, -_axisValue * 3.f, 0.f));

		} // end if

	} // end if

} // void MoveMouseX()

void AStageCube_1::MoveMouseY(const float _axisValue)
{
	if (isDraggingGuideLine && mCurrentSelectedGuideLine != NULL)
	{
		if (mCurrentSelectedGuideLine == mGuideLineZaxis)
		{
			// wiil be strange when turn on other side
			mCurrentSelectedGuideLine->mRootComponent->SetRelativeRotation(MyCombineRotators(mCurrentSelectedGuideLine->mRootComponent->GetRelativeRotation(), FRotator(0.f, -_axisValue * guideLineTurnningScale, 0.f)));
		} // end if
		else if (mCurrentSelectedGuideLine == mGuideLineXaxis)
		{
			// wiil be strange when turn on other side
			mCurrentSelectedGuideLine->mRootComponent->SetRelativeRotation(MyCombineRotators(mCurrentSelectedGuideLine->mRootComponent->GetRelativeRotation(), FRotator(0.f, 0.f, _axisValue * guideLineTurnningScale)));
		} // end if
		else if (mCurrentSelectedGuideLine == mGuideLineYaxis)
		{
			// =================  Caution  8/21  ================
			// ガイドライン Y のActorのRotation設定は他のガイドラインと違います、要注意です。
			// (元々はPitchを回転すると、オイラー角の制限があった為に、こうになった)
			// wiil be strange when turn on other side
			mCurrentSelectedGuideLine->mRootComponent->SetRelativeRotation(MyCombineRotators(mCurrentSelectedGuideLine->mRootComponent->GetRelativeRotation(), FRotator(0.f, 0.f, -_axisValue * guideLineTurnningScale)));
		} // end if

	} // end if
	else if (isMovingCamera)
	{
		// will affect roll, don't use
		// mSpringArm->AddRelativeRotation(FRotator(_axisValue * mCameraTurnScaleY, 0.f, 0.f));

		// Cameraを回転
		// MyCombineRotatorsを使って調整する
		FRotator tempR = MyCombineRotators(mSpringArm->GetRelativeRotation(), FRotator(_axisValue * mCameraTurnScaleY, 0.f, 0.f));
		tempR.Roll = 0.f;

		mCameraTurnScaleY = AdjustCameraTurnScale(tempR.Pitch);

		// Pitchが90度ならないように(オイラー角の制限)
		if (tempR.Pitch > 85.f)
			tempR.Pitch = 85.f;
		else if (tempR.Pitch < -85.f)
			tempR.Pitch = -85.f;

		mSpringArm->SetRelativeRotation(tempR);


		// Cubeを回転
		//mCubesRootComponent->AddRelativeRotation(FRotator(-_axisValue * mCameraTurnScaleY, 0.f, 0.f));

		// will affect roll, don't use
		// mCubesRootComponent->AddLocalRotation(FRotator(-_axisValue * 3.f, 0.f, 0.f));

	} // end if

	// GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Emerald, FString::SanitizeFloat(_axisValue));

} // void MoveMouseY()

void AStageCube_1::MouseLeftButtonPressed()
{
	if (isSelectingCube && isSelectingGuideLine)
	{
		mStartRotateDegree = mCurrentSelectedGuideLine->GetActorRotation();
		isDraggingGuideLine = true;
	} // end if()
	else
	{
		isMovingCamera = true;
	} // end else

	 GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, FString::Printf(TEXT("Left  Pressed!!")));

} // void MouseLeftButtonPressed()


void AStageCube_1::MouseLeftButtonReleased()
{
	if (isDraggingGuideLine && mCurrentSelectedGuideLine != NULL)
	{
		NormalizeGuideRotation();

		// dont need
		if (mCurrentSelectedGuideLine == mGuideLineZaxis)
		{
			float newRot = 0.f;
			// mCurrentSelectedGuideLine->AddActorLocalRotation(FRotator(_axisValue * guideLineTurnningScale, 0.f, 0.f));

		} // end if()


		isDraggingGuideLine = false;
	} // end if
	else
	{
		isMovingCamera = false;
	} // end else

	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, FString::Printf(TEXT("Left released")));

} // void MouseLeftButtonReleased()


void AStageCube_1::SwapCoordinate(ACubeUnit* _A, ACubeUnit* _B)
{
	if (_A == NULL || _B == NULL)
		return;

	Swap(_A->mXCoordinate, _B->mXCoordinate);
	Swap(_A->mYCoordinate, _B->mYCoordinate);
	Swap(_A->mZCoordinate, _B->mZCoordinate);

}

// カメラ回転スケールを調整する
// 50度から85度、3~0.2倍
float AStageCube_1::AdjustCameraTurnScale(const float _inPitch)
{
	float ratio = 3.f;

	if (_inPitch >= 50.f)
		ratio = 3.0f - (((_inPitch - 50.f) / 35.f) * 2.8f );
	else if (_inPitch <= -50.f)
		ratio = 3.0f - (((-50.f - _inPitch ) / 35.f) * 2.8f);

	return ratio;

} // float AdjustCameraTurnScale()

// For Debug (Debug用 + Editorで見える)ため、回転後の場所を三つのArray再配置
void AStageCube_1::Replace3Array()
{

	for (int i = 0; i < 3; ++i)
	{
		int cntA = 0;
		for (int j = 0; j < 3; ++j)
		{
			for (int k = 0; k < 3; ++k)
			{
				if ( i == 0)
					d1Cube[cntA] = CubeArray3D[i][j][k];
				else if (i == 1)
					d2Cube[cntA] = CubeArray3D[i][j][k];
				else if (i == 2)
					d3Cube[cntA] = CubeArray3D[i][j][k];

				++cntA;
			} // end for
		} // end for
	} // end for

} // void Replace3Array()


// cant active this　but doesnt need it anymore
// change to cube unit -> get parent? -> cast and call this
void AStageCube_1::OnSelected(AActor* Target, FKey ButtonPressed)
{

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, FString::Printf(TEXT("ALL Clicked! (StageCube_1)")));


	if (mCurrentSelectedCube != NULL)
	{
		mCurrentSelectedCube->mIsSelected = false;
		mCurrentSelectedCube->ChangeMaterialFunc();

		mCurrentSelectedCube = Cast<ACubeUnit>(Target);

	} // end if()
	else
	{
		mCurrentSelectedCube = Cast<ACubeUnit>(Target);
	} // end else

} // void OnSelected()


// mode 0 = XY-Plane(Z-axis),  1 = YZ-Plane(X-axis),  2 = XZ-Plane(Y-axis)
void AStageCube_1::AttachToGuideLine( const int mode)
{
	FAttachmentTransformRules AttachRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false);

	// 配列のindex

	if (mCurrentSelectedCube != NULL)
	{
		// 配列のindex
		// i j k  will be Z X Y of the array
		// can get the position where line to move and who to attach
		int i = mCurrentSelectedCube->mZCoordinate;
		int j = mCurrentSelectedCube->mXCoordinate;
		int k = mCurrentSelectedCube->mYCoordinate;

		if (CubeArray3D[i][j][k] != NULL)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("===== %s"), *CubeArray3D[i][j][k]->GetName()));

			// === guide Z =====
			if (mode == 0)
			{
				for (int x = 0; x < 3; ++x)
				{
					for (int y = 0; y < 3; ++y)
					{
						if (CubeArray3D[i][x][y] != NULL)
						{
							if (mGuideLineZaxis != NULL)
							{
								CubeArray3D[i][x][y]->AttachToActor(mGuideLineZaxis, AttachRules);

								GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("Attach  %s  To GuideLineZ"), *CubeArray3D[i][x][y]->GetName()));

							} // end if()
							else GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("mGuideLineZaxis is NULL")));
						} // end if()
					} // end for
				} // end for
			} // end if()
			//  === guide X =====
			else if (mode == 1)
			{
				for (int z = 0; z < 3; ++z)
				{
					for (int y = 0; y < 3; ++y)
					{
						if (CubeArray3D[z][j][y] != NULL)
						{
							if (mGuideLineXaxis != NULL)
							{
								CubeArray3D[z][j][y]->AttachToActor(mGuideLineXaxis, AttachRules);
								GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Attach  %s  To GuideLineX"), *CubeArray3D[z][j][y]->GetName()));
							} // end if()
						} // end if()
						else GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Detach Failed! One cube is NULL")));
					} // end for
				} // end for
			} // end if()
			//  === guide Y =====
			else if (mode == 2)
			{
				for (int z = 0; z < 3; ++z)
				{
					for (int x = 0; x < 3; ++x)
					{
						if (CubeArray3D[z][x][k] != NULL)
						{
							if (mGuideLineYaxis != NULL)
							{
								CubeArray3D[z][x][k]->AttachToActor(mGuideLineYaxis, AttachRules);
								GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Attach  %s  To GuideLineY"), *CubeArray3D[z][x][k]->GetName()));
							} // end if()
						} // end if()
						else GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Detach Failed! One cube is NULL")));
					} // end for
				} // end for
			} // end if()
		} // end if()
		else GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Attach's Cube3D is NULL")));

	} // end if()
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("AttachToGuideLine's Seleted Cube is NULL")));
	} // end else

} // void AttachToGuideLine()

void AStageCube_1::DetachFromGuideLine()
{
	FAttachmentTransformRules AttachRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false);

	if (mCurrentSelectedCube != NULL)
	{
		// i j k  will be Z X Y of the array
		// can get the position where line to move and who to attach

		// ===== guide Z =====
		if (mCurrentSelectedGuideLine == mGuideLineZaxis)
		{
			int i = mCurrentSelectedCube->mZCoordinate;

			for (int x = 0; x < 3; ++x)
			{
				for (int y = 0; y < 3; ++y)
				{
					if (CubeArray3D[i][x][y] != NULL) 
					{
						CubeArray3D[i][x][y]->AttachToComponent(mCubesRootComponent, AttachRules);

						GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Detach  %s  From GuideLineZ"), *CubeArray3D[i][x][y]->GetName()));

					} // end if()
					else GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Detach Failed! One cube is NULL")));
				} // end for
			} // end for
		} // end if()
		// ===== guide X =====
		else if (mCurrentSelectedGuideLine == mGuideLineXaxis)
		{
			int j = mCurrentSelectedCube->mXCoordinate;

			for (int z = 0; z < 3; ++z)
			{
				for (int y = 0; y < 3; ++y)
				{
					if (CubeArray3D[z][j][y] != NULL)
					{
						CubeArray3D[z][j][y]->AttachToComponent(mCubesRootComponent, AttachRules);
						GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Detach  %s  From GuideLineX"), *CubeArray3D[z][j][y]->GetName()));

					} // end if()
					else GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Detach Failed! One cube is NULL")));
				} // end for
			} // end for
		} // end if()
		// ===== guide Y =====
		else if (mCurrentSelectedGuideLine == mGuideLineYaxis)
		{
			int k = mCurrentSelectedCube->mYCoordinate;

			for (int z = 0; z < 3; ++z)
			{
				for (int x = 0; x < 3; ++x)
				{
					if (CubeArray3D[z][x][k] != NULL)
					{
						CubeArray3D[z][x][k]->AttachToComponent(mCubesRootComponent, AttachRules);
						GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Detach  %s  From GuideLineY"), *CubeArray3D[z][x][k]->GetName()));

					} // end if()
					else GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Detach Failed! One cube is NULL")));
				} // end for
			} // end for
		} // end if()

	} // end if()
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("DetachFromGuideLine's Seleted Cube is NULL")));
	} // end else

} // void DetachFromGuideLine()

void AStageCube_1::SetGuideLinePosition()
{
	if (mCurrentSelectedCube != NULL)
	{
		FVector tempRelativeLocation = mCurrentSelectedCube->GetActorLocation() - this->GetActorLocation() ;

		if (mGuideLineZaxis != NULL)
			mGuideLineZaxis->SetActorRelativeLocation(FVector(0.f, 0.f, tempRelativeLocation.Z));

		if (mGuideLineXaxis != NULL)
			mGuideLineXaxis->SetActorRelativeLocation(FVector(tempRelativeLocation.X, 0.f, 0.f));

		if (mGuideLineYaxis != NULL)
			mGuideLineYaxis->SetActorRelativeLocation(FVector(0.f, tempRelativeLocation.Y, 0.f));

		// set visibility
		if (mGuideLineZaxis->mIsVisible == false)
		{
			mGuideLineZaxis->mIsVisible = true;
			mGuideLineZaxis->ChangeVisibilityFunc();
		} // end if()
	} // end if

} // void SetGuideLinePosition()

void AStageCube_1::ChangeAllGuideLinesVisibility( const bool isVisible)
{
	if (mGuideLineXaxis != NULL)
	{
		mGuideLineXaxis->mIsVisible = isVisible;
		mGuideLineXaxis->ChangeVisibilityFunc();
	} // end if()

	if (mGuideLineYaxis != NULL)
	{
		mGuideLineYaxis->mIsVisible = isVisible;
		mGuideLineYaxis->ChangeVisibilityFunc();
	} // end if()

	if (mGuideLineZaxis != NULL)
	{
		mGuideLineZaxis->mIsVisible = isVisible;
		mGuideLineZaxis->ChangeVisibilityFunc();
	} // end if()

} // void ChangeAllGuideLinesVisibility()

void AStageCube_1::DeSelectCubeAndGuide(bool deSelectCube, bool deSelectGuide)
{
	if (deSelectCube == true)
	{
		if (mCurrentSelectedCube != NULL)
		{
			mCurrentSelectedCube->mIsSelected = false;
			mCurrentSelectedCube->ChangeMaterialFunc();
			mCurrentSelectedCube = NULL;
			SetSelectingCube(false);
		} // end if
	} // end if


	if (deSelectGuide == true)
	{
		if (mCurrentSelectedGuideLine != NULL)
		{
			DetachFromGuideLine();
			SetSelectingGuideLine(false);
			mCurrentSelectedGuideLine->mIsSelected = false;
			mCurrentSelectedGuideLine->ChangeMaterialFunc();
			mCurrentSelectedGuideLine = NULL;
		} // end if
	} // end if

	ChangeAllGuideLinesVisibility(false);

} // void DeSelectCubeAndGuide()

void AStageCube_1::SetSelectingGuideLine( const bool isSelect)
{
	isSelectingGuideLine = isSelect;

} // void SetSelectingGuideLine()

void AStageCube_1::SetSelectingCube( const bool isSelect)
{
	isSelectingCube = isSelect;

} // void SetSelectingCube()

void AStageCube_1::NormalizeGuideRotation()
{
	// ===================================== ガイドライン　Z =================================
	if (mCurrentSelectedGuideLine == mGuideLineZaxis)
	{
		// 回転開始と完了の差から、基本回転するの角度を決める
		float basicDegree = round((mCurrentSelectedGuideLine->GetActorRotation().Yaw - mStartRotateDegree.Yaw) / 90.f) * 90.f;

		// 角度確認用
		// GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Turquoise, FString::SanitizeFloat(basicDegree));

		mCurrentSelectedGuideLine->mRootComponent->SetRelativeRotation(FRotator(0.f, mStartRotateDegree.Yaw + basicDegree, 0.f));
		
		// 配列の回転
		if (basicDegree == 90.f || basicDegree == -270.f)
		{
			RoatateTheCubesRight90(0);
		} // end if
		else if (basicDegree == 180.f || basicDegree == -180.f)
		{
			RoatateTheCubes180(0);
		} // end if
		else if (basicDegree == 270.f || basicDegree == -90.f)
		{
			RoatateTheCubesLeft90(0);
		} // end if

	} // end if()
	// ===================================== ガイドライン　X =================================
	else if (mCurrentSelectedGuideLine == mGuideLineXaxis)
	{
		// 回転開始と完了の差から、基本回転するの角度を決める
		float basicDegree = round((mCurrentSelectedGuideLine->GetActorRotation().Roll - mStartRotateDegree.Roll) / 90.f) * 90.f;

		// 角度確認用
		// GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Turquoise, FString::SanitizeFloat(basicDegree));

		mCurrentSelectedGuideLine->mRootComponent->SetRelativeRotation(FRotator(0.f, 0.f, mStartRotateDegree.Roll + basicDegree));

		// 配列の回転
		if (basicDegree == 90.f || basicDegree == -270.f)
		{
			RoatateTheCubesRight90(1);
		} // end if
		else if (basicDegree == 180.f || basicDegree == -180.f)
		{
			RoatateTheCubes180(1);
		} // end if
		else if (basicDegree == 270.f || basicDegree == -90.f)
		{
			RoatateTheCubesLeft90(1);
		} // end if

	} // end if()
	// ===================================== ガイドライン　Y =================================
	else if (mCurrentSelectedGuideLine == mGuideLineYaxis)
	{
		// 回転開始と完了の差から、基本回転するの角度を決める
		float basicDegree = round((mCurrentSelectedGuideLine->GetActorRotation().Roll - mStartRotateDegree.Roll) / 90.f) * 90.f;

		// 角度確認用
		// GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Turquoise, FString::SanitizeFloat(basicDegree));

		mCurrentSelectedGuideLine->mRootComponent->SetRelativeRotation(FRotator(0.f, 90.f, mStartRotateDegree.Roll + basicDegree));

		// 配列の回転
		if (basicDegree == 90.f || basicDegree == -270.f)
		{
			RoatateTheCubesRight90(2);
		} // end if
		else if (basicDegree == 180.f || basicDegree == -180.f)
		{
			RoatateTheCubes180(2);
		} // end if
		else if (basicDegree == 270.f || basicDegree == -90.f)
		{
			RoatateTheCubesLeft90(2);
		} // end if

	} // end if()

	SetGuideLinePosition();

} // void NormalizeGuideRotation()

void AStageCube_1::ManageGuideLineRotateResultToArray()
{
} // void ManageGuideLineRotateResultToArray()


