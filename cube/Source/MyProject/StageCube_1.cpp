//---------------------------------------------------------------------------------
// �t�@�C��		�FStageCube_1.cpp
// �T�v			�F����Cube�̏���
// �쐬��		�F19CU0236 �щ_�� 
// �쐬��		�F2021/08/11		����Cube�̊�{�\������
// �X�V��		�F2021/08/12		�P��Cube�����Ɣz�����A�I���@�\
//				�F2021/08/13		��]�̂��ߔz��ɂ��Matrix�v�Z
//				�F2021/08/13		��]�p�K�C�h���C���̐����A�I���@�\�A�A�^�b�`�@�\
//				�F2021/08/14		��]�p�K�C�h���C���̎����ǉ��A�f�^�b�`�@�\
//				�F2021/08/16		�P��Cube��I���������ɉ�]�p�K�C�h���C���̏���(�z�u�A����)
//				�F2021/08/17		����]90�x(=�E��]270�x)�Ɖ�]180�x�ǉ�
//				�F2021/08/18		����Cube�̉�]����ǉ��A�K�C�h���C���̉�]����ǉ�
//				�F2021/08/19		Camera��]�̋����A�K�C�h���C����]�̋���
//				�F2021/08/20		��]�̏C���A��]�v�Z���@��Quaternion����v�Z�ɕύX
//				�F2021/08/21		�K�C�h���C��Y�̏C��
//---------------------------------------------------------------------------------

#include "StageCube_1.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine.h"

// �R���X�g���N�^
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

	// �X�v�����O�A�[���̃I�u�W�F�N�g�𐶐�
	mSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("mSpringArm"));

	// �X�v�����O�A�[����RootComponent���A�^�b�`
	mSpringArm->SetupAttachment(RootComponent);

	// �J�����ƃv���C���[�̋���
	mSpringArm->TargetArmLength = 800.f;

	// �J�����̎q���W�����e�X�g���s������ݒ�
	mSpringArm->bDoCollisionTest = false;

	// �J�����𐶐�
	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("mCamera"));

	// �J�����ɃX�v�����O�A�[�����A�^�b�`
	mCamera->SetupAttachment(mSpringArm, USpringArmComponent::SocketName);


	OnClicked.AddUniqueDynamic(this, &AStageCube_1::OnSelected);

}

// �f�X�g���N�^
AStageCube_1::~AStageCube_1()
{
}

// ���s���Ɉ�x�Ă΂��
void AStageCube_1::BeginPlay()
{
	Super::BeginPlay();
	
	mSpringArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));

	// FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);

	// ================================ Cube �̐���  ================================

	bp_CubeUnit = TSoftClassPtr<AActor>(FSoftObjectPath(*path)).LoadSynchronous();	// path�ɂ���BP���擾

	if (bp_CubeUnit != nullptr)
	{
		int counter = 1;							// �v����
		int remainder = 0;							// �]��

		// ���_�̈ʒu
		float x1 = 100.f;
		float y1 = -100.f;
		float z1 = 100.f;

		// �z��p�̍��W�ʒu
		int xC = 0;		// ���WX
		int yC = 0;		// ���WY
		int zC = 0;		// ���WZ

		ACubeUnit* cubeGen;								// Cube Actor�����p

		TArray< TArray<ACubeUnit*>> tempArray2D;		// ���ۑ��p2D Array
		TArray<ACubeUnit*> tempArray1D;					// ���ۑ��pArray 

		// *** For test position 8/12
		int tempSerialNum = 1000;

		for (int i = 0; i <= 26; ++i, ++counter, ++yC) {

			cubeGen = GetWorld()->SpawnActor<ACubeUnit>(bp_CubeUnit);				// �X�|�[��Cube Actor

			// �z��̈ʒu�v�Z
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

			// ===================  Debug�p + Editor�Ō�����  =======================
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

			// ���̍s
			if (counter % 3 == 0 )
			{
				tempArray2D.Add(tempArray1D);
				tempArray1D.Empty();

				x1 -= 100.f;

				++xC;
				yC = -1;		// for�́{1�ɂ���

				// *** For test position 8/12
				tempSerialNum += 7;
			}  // end if()
			// ���̊K�w
			if (counter % 9 == 0)
			{
				CubeArray3D.Add(tempArray2D);
				tempArray2D.Empty();

				z1 -= 100.f;
				x1 = 100.f;
				y1 = -100.f;

				xC = 0;
				yC = -1;		// for�́{1�ɂ���
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


	// ==========================  Guide Line �̐���  ==========================

	FString pathGL = "Blueprint'/Game/BP/BP_GuideLineZ.BP_GuideLineZ_C'";
	TSubclassOf<class AActor> bp_GuideLineZ = TSoftClassPtr<AActor>(FSoftObjectPath(*pathGL)).LoadSynchronous();	// path�ɂ���BP���擾

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
	// �K�C�h���C�� Y ��Actor��Rotation�ݒ�͑��̃K�C�h���C���ƈႢ�܂��A�v���ӂł��B
	// (���X��Pitch����]����ƁA�I�C���[�p�̐������������ׂɁA�����ɂȂ���)
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

	// �}�E�X�̃N���b�N�C�x���g�p
	APlayerController* myPlayerController =	UGameplayStatics::GetPlayerController(GetWorld(), 0);

	myPlayerController->bShowMouseCursor = true;
	myPlayerController->bEnableMouseOverEvents = true;
	myPlayerController->bEnableClickEvents = true;
	// for test
	// RoatateTheCubesRight90( 1);
	// Replace3Array();
	// RoatateTheCubesLeft90(0);


} // BeginPlay()


// ���t���[���Ă΂��
void AStageCube_1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

} // Tick

// �e���͊֌W���\�b�h�Ƃ̃o�C���h����
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
// �E��]90�x�A���̉�]�����v�m�F
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
		// 8/13)) �I�����ꂽcube�̈ʒu�擾
		int tempZ = 0;
		if (mCurrentSelectedCube != NULL)
			tempZ = mCurrentSelectedCube->mZCoordinate;
		else {
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("RoatateTheCubesRight90 : mCurrentSelectedCube is NULL")));
			return;
		} // end else

		// ��Ίp���]�u�s�� Transposed Matrix
		for (int i = 0; i < totalCol; ++i) {
			for (int j = 0; j < i; ++j) {
				Swap(CubeArray3D[tempZ][i][j], CubeArray3D[tempZ][j][i]);
				SwapCoordinate(CubeArray3D[tempZ][i][j], CubeArray3D[tempZ][j][i]);
			} // end for()
		} // end for()

		// ���E����������
		for (int i = 0; i < totalCol / 2; ++i) {
			for (int j = 0; j < totalCol; ++j) {
				Swap(CubeArray3D[tempZ][j][i], CubeArray3D[tempZ][j][totalCol - i - 1]);
				SwapCoordinate(CubeArray3D[tempZ][j][i], CubeArray3D[tempZ][j][totalCol - i - 1]);
			} // end for()
		} // end for()

	} // end if()
	else if (mode == 1)
	{
		// �I�����ꂽcube�̈ʒu�擾
		int tempX = 0;
		if (mCurrentSelectedCube != NULL)
			tempX = mCurrentSelectedCube->mXCoordinate;
		else {
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("RoatateTheCubesRight90 : mCurrentSelectedCube is NULL")));
			return;
		} // end else

		// ��Ίp���]�u�s�� Transposed Matrix
		for (int i = 0; i < totalCol; ++i) {
			for (int j = 0; j < i; ++j) {
				Swap(CubeArray3D[i][tempX][j], CubeArray3D[j][tempX][i]);
				SwapCoordinate(CubeArray3D[i][tempX][j], CubeArray3D[j][tempX][i]);
			} // end for()
		} // end for()

		// ���E����������
		for (int i = 0; i < totalCol / 2; ++i) {
			for (int j = 0; j < totalCol; ++j) {
				Swap(CubeArray3D[j][tempX][i], CubeArray3D[j][tempX][totalCol - i - 1]);
				SwapCoordinate(CubeArray3D[j][tempX][i], CubeArray3D[j][tempX][totalCol - i - 1]);
			} // end for()
		} // end for()

	} // end if()
	else if (mode == 2)
	{
		// �I�����ꂽcube�̈ʒu�擾
		int tempY = 0;
		if (mCurrentSelectedCube != NULL)
			tempY = mCurrentSelectedCube->mYCoordinate;
		else {
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("RoatateTheCubesRight90 : mCurrentSelectedCube is NULL")));
			return;
		} // end else

		// ��Ίp���]�u�s�� Transposed Matrix
		for (int i = 0; i < totalCol; ++i) {
			for (int j = 0; j < i; ++j) {
				Swap(CubeArray3D[i][j][tempY], CubeArray3D[j][i][tempY]);
				SwapCoordinate(CubeArray3D[i][j][tempY], CubeArray3D[j][i][tempY]);
			} // end for()
		} // end for()

		// ���E����������
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

	// Debug�p�@editor�Ŋm�F�p
	Replace3Array();

} // void RoatateTheCubesRight90()


/*
==================================  ���ؗp  ==================================
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


==================================  ���ؗp  ==================================
*/


// ����]90�x(=�E��]270�x)
// mode 0 = XY-Plane(Z-axis),  1 = YZ-Plane(X-axis),  2 = XZ-Plane(Y-axis)
// 3D Array [Z][X][Y]
//         Dep Row Col
void AStageCube_1::RoatateTheCubesLeft90( const int mode)
{
	// CubeArray3D
	int totalCol = 3;

	if (mode == 0)
	{
		// �I�����ꂽcube�̈ʒu�擾
		int tempZ = 0;
		if (mCurrentSelectedCube != NULL)
			tempZ = mCurrentSelectedCube->mZCoordinate;
		else {
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("RoatateTheCubesLeft90 : mCurrentSelectedCube is NULL")));
			return;
		} // end else

		// ��Ίp���]�u�s�� Transposed Matrix
		for (int i = 0; i < totalCol; ++i) {
			for (int j = 0; j < i; ++j) {
				Swap(CubeArray3D[tempZ][i][j], CubeArray3D[tempZ][j][i]);
				SwapCoordinate(CubeArray3D[tempZ][i][j], CubeArray3D[tempZ][j][i]);
			} // end for()
		} // end for()

		// �㉺��������
		for (int i = 0; i < totalCol / 2; ++i) {
			for (int j = 0; j < totalCol; ++j) {
				Swap(CubeArray3D[tempZ][i][j], CubeArray3D[tempZ][totalCol - i - 1][j]);
				SwapCoordinate(CubeArray3D[tempZ][i][j], CubeArray3D[tempZ][totalCol - i - 1][j]);
			} // end for()
		} // end for()

	} // end if()
	else if (mode == 1)
	{
		// �I�����ꂽcube�̈ʒu�擾
		int tempX = 0;
		if (mCurrentSelectedCube != NULL)
			tempX = mCurrentSelectedCube->mXCoordinate;
		else {
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("RoatateTheCubesLeft90 : mCurrentSelectedCube is NULL")));
			return;
		} // end else
		
		// ��Ίp���]�u�s�� Transposed Matrix
		for (int i = 0; i < totalCol; ++i) {
			for (int j = 0; j < i; ++j) {
				Swap(CubeArray3D[i][tempX][j], CubeArray3D[j][tempX][i]);
				SwapCoordinate(CubeArray3D[i][tempX][j], CubeArray3D[j][tempX][i]);
			} // end for()
		} // end for()

		// �㉺��������
		for (int i = 0; i < totalCol / 2; ++i) {
			for (int j = 0; j < totalCol; ++j) {
				Swap(CubeArray3D[i][tempX][j], CubeArray3D[totalCol - i - 1][tempX][j]);
				SwapCoordinate(CubeArray3D[i][tempX][j], CubeArray3D[totalCol - i - 1][tempX][j]);
			} // end for()
		} // end for()

	} // end if()
	else if (mode == 2)
	{
		// �I�����ꂽcube�̈ʒu�擾
		int tempY = 0;
		if (mCurrentSelectedCube != NULL)
			tempY = mCurrentSelectedCube->mYCoordinate;
		else {
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("RoatateTheCubesLeft90 : mCurrentSelectedCube is NULL")));
			return;
		} // end else

		// ��Ίp���]�u�s�� Transposed Matrix
		for (int i = 0; i < totalCol; ++i) {
			for (int j = 0; j < i; ++j) {
				Swap(CubeArray3D[i][j][tempY], CubeArray3D[j][i][tempY]);
				SwapCoordinate(CubeArray3D[i][j][tempY], CubeArray3D[j][i][tempY]);
			} // end for()
		} // end for()

		// �㉺��������
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

	// Debug�p�@editor�Ŋm�F�p
	Replace3Array();

} // void RoatateTheCubesLeft90()


// ��]180�x
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

		// �㉺��������
		for (int i = 0; i < totalCol / 2; ++i) {
			for (int j = 0; j < totalCol; ++j) {
				Swap(CubeArray3D[tempZ][i][j], CubeArray3D[tempZ][totalCol - i - 1][j]);
				SwapCoordinate(CubeArray3D[tempZ][i][j], CubeArray3D[tempZ][totalCol - i - 1][j]);
			} // end for()
		} // end for()

		// ���E����������
		for (int i = 0; i < totalCol / 2; ++i) {
			for (int j = 0; j < totalCol; ++j) {
				Swap(CubeArray3D[tempZ][j][i], CubeArray3D[tempZ][j][totalCol - i - 1]);
				SwapCoordinate(CubeArray3D[tempZ][j][i], CubeArray3D[tempZ][j][totalCol - i - 1]);
			} // end for()
		} // end for()

	} // end if()
	else if (mode == 1)
	{
		// �I�����ꂽcube�̈ʒu�擾
		int tempX = 0;
		if (mCurrentSelectedCube != NULL)
			tempX = mCurrentSelectedCube->mXCoordinate;
		else {
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("RoatateTheCubesLeft90 : mCurrentSelectedCube is NULL")));
			return;
		} // end else

		// �㉺��������
		for (int i = 0; i < totalCol / 2; ++i) {
			for (int j = 0; j < totalCol; ++j) {
				Swap(CubeArray3D[i][tempX][j], CubeArray3D[totalCol - i - 1][tempX][j]);
				SwapCoordinate(CubeArray3D[i][tempX][j], CubeArray3D[totalCol - i - 1][tempX][j]);
			} // end for()
		} // end for()

		// ���E����������
		for (int i = 0; i < totalCol / 2; ++i) {
			for (int j = 0; j < totalCol; ++j) {
				Swap(CubeArray3D[j][tempX][i], CubeArray3D[j][tempX][totalCol - i - 1]);
				SwapCoordinate(CubeArray3D[j][tempX][i], CubeArray3D[j][tempX][totalCol - i - 1]);
			} // end for()
		} // end for()

	} // end if()
	else if (mode == 2)
	{
		// �I�����ꂽcube�̈ʒu�擾
		int tempY = 0;
		if (mCurrentSelectedCube != NULL)
			tempY = mCurrentSelectedCube->mYCoordinate;
		else {
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("RoatateTheCubesLeft90 : mCurrentSelectedCube is NULL")));
			return;
		} // end else

		// �㉺��������
		for (int i = 0; i < totalCol / 2; ++i) {
			for (int j = 0; j < totalCol; ++j) {
				Swap(CubeArray3D[i][j][tempY], CubeArray3D[totalCol - i - 1][j][tempY]);
				SwapCoordinate(CubeArray3D[i][j][tempY], CubeArray3D[totalCol - i - 1][j][tempY]);
			} // end for()
		} // end for()

		// ���E����������
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

	// Debug�p�@editor�Ŋm�F�p
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
				// �K�C�h���C�� Y ��Actor��Rotation�ݒ�͑��̃K�C�h���C���ƈႢ�܂��A�v���ӂł��B
				// (���X��Pitch����]����ƁA�I�C���[�p�̐������������ׂɁA�����ɂȂ���)
				// wiil be strange when turn on other side
				mCurrentSelectedGuideLine->mRootComponent->SetRelativeRotation(MyCombineRotators(mCurrentSelectedGuideLine->mRootComponent->GetRelativeRotation(), FRotator(0.f, 0.f, -_axisValue * guideLineTurnningScale)));
			} // end if

		} // end if
		else if (isMovingCamera)
		{
			// will affect roll, don't use
			// mSpringArm->AddRelativeRotation(FRotator(0.f, _axisValue * mCameraTurnScaleX, 0.f));

			// Camera����]
			// MyCombineRotators���g���Ē�������
			FRotator tempR = MyCombineRotators(mSpringArm->GetRelativeRotation(), FRotator(0.f, _axisValue * mCameraTurnScaleX, 0.f));
			tempR.Roll = 0.f;
		
			mCameraTurnScaleX = AdjustCameraTurnScale(tempR.Pitch);

			// Pitch��90�x�Ȃ�Ȃ��悤��(�I�C���[�p�̐���)
			if (tempR.Pitch > 85.f)
				tempR.Pitch = 85.f;
			else if (tempR.Pitch < -85.f)
				tempR.Pitch = -85.f;

			mSpringArm->SetRelativeRotation(tempR);

			// Cube����]
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
			// �K�C�h���C�� Y ��Actor��Rotation�ݒ�͑��̃K�C�h���C���ƈႢ�܂��A�v���ӂł��B
			// (���X��Pitch����]����ƁA�I�C���[�p�̐������������ׂɁA�����ɂȂ���)
			// wiil be strange when turn on other side
			mCurrentSelectedGuideLine->mRootComponent->SetRelativeRotation(MyCombineRotators(mCurrentSelectedGuideLine->mRootComponent->GetRelativeRotation(), FRotator(0.f, 0.f, -_axisValue * guideLineTurnningScale)));
		} // end if

	} // end if
	else if (isMovingCamera)
	{
		// will affect roll, don't use
		// mSpringArm->AddRelativeRotation(FRotator(_axisValue * mCameraTurnScaleY, 0.f, 0.f));

		// Camera����]
		// MyCombineRotators���g���Ē�������
		FRotator tempR = MyCombineRotators(mSpringArm->GetRelativeRotation(), FRotator(_axisValue * mCameraTurnScaleY, 0.f, 0.f));
		tempR.Roll = 0.f;

		mCameraTurnScaleY = AdjustCameraTurnScale(tempR.Pitch);

		// Pitch��90�x�Ȃ�Ȃ��悤��(�I�C���[�p�̐���)
		if (tempR.Pitch > 85.f)
			tempR.Pitch = 85.f;
		else if (tempR.Pitch < -85.f)
			tempR.Pitch = -85.f;

		mSpringArm->SetRelativeRotation(tempR);


		// Cube����]
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

// �J������]�X�P�[���𒲐�����
// 50�x����85�x�A3~0.2�{
float AStageCube_1::AdjustCameraTurnScale(const float _inPitch)
{
	float ratio = 3.f;

	if (_inPitch >= 50.f)
		ratio = 3.0f - (((_inPitch - 50.f) / 35.f) * 2.8f );
	else if (_inPitch <= -50.f)
		ratio = 3.0f - (((-50.f - _inPitch ) / 35.f) * 2.8f);

	return ratio;

} // float AdjustCameraTurnScale()

// For Debug (Debug�p + Editor�Ō�����)���߁A��]��̏ꏊ���O��Array�Ĕz�u
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


// cant active this�@but doesnt need it anymore
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

	// �z���index

	if (mCurrentSelectedCube != NULL)
	{
		// �z���index
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
	// ===================================== �K�C�h���C���@Z =================================
	if (mCurrentSelectedGuideLine == mGuideLineZaxis)
	{
		// ��]�J�n�Ɗ����̍�����A��{��]����̊p�x�����߂�
		float basicDegree = round((mCurrentSelectedGuideLine->GetActorRotation().Yaw - mStartRotateDegree.Yaw) / 90.f) * 90.f;

		// �p�x�m�F�p
		// GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Turquoise, FString::SanitizeFloat(basicDegree));

		mCurrentSelectedGuideLine->mRootComponent->SetRelativeRotation(FRotator(0.f, mStartRotateDegree.Yaw + basicDegree, 0.f));
		
		// �z��̉�]
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
	// ===================================== �K�C�h���C���@X =================================
	else if (mCurrentSelectedGuideLine == mGuideLineXaxis)
	{
		// ��]�J�n�Ɗ����̍�����A��{��]����̊p�x�����߂�
		float basicDegree = round((mCurrentSelectedGuideLine->GetActorRotation().Roll - mStartRotateDegree.Roll) / 90.f) * 90.f;

		// �p�x�m�F�p
		// GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Turquoise, FString::SanitizeFloat(basicDegree));

		mCurrentSelectedGuideLine->mRootComponent->SetRelativeRotation(FRotator(0.f, 0.f, mStartRotateDegree.Roll + basicDegree));

		// �z��̉�]
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
	// ===================================== �K�C�h���C���@Y =================================
	else if (mCurrentSelectedGuideLine == mGuideLineYaxis)
	{
		// ��]�J�n�Ɗ����̍�����A��{��]����̊p�x�����߂�
		float basicDegree = round((mCurrentSelectedGuideLine->GetActorRotation().Roll - mStartRotateDegree.Roll) / 90.f) * 90.f;

		// �p�x�m�F�p
		// GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Turquoise, FString::SanitizeFloat(basicDegree));

		mCurrentSelectedGuideLine->mRootComponent->SetRelativeRotation(FRotator(0.f, 90.f, mStartRotateDegree.Roll + basicDegree));

		// �z��̉�]
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


