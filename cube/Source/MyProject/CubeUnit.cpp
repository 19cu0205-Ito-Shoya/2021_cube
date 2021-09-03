//-------------------------------------------------------------------
// �t�@�C��		�FCubeUnit.cpp
// �T�v			�F�P��Cube�̏���
// �쐬��		�F19CU0236 �щ_�� 
// �쐬��		�F2021/08/11		�P��Cube�̊�{�\������
// �X�V��		�F2021/08/12		�I���@�\�����A�}�e���A���ς����
//				�F2021/08/15		�I���@�\�̉�]�p�K�C�h���C���̏����ǉ�
//				�F2021/08/23		�I���@�\�̃C���v�b�g�C�x���g��StageCube�Ɉړ�����
//				�F2021/08/25		�}�E�X�J�[�\���d�˂鎞�̃}�e���A���ύX
//				�F2021/08/26		�}�e���A���ύX�ł��邩���f�ǉ�
//				�F2021/09/03		�}�e���A�����}�e���A���C���^�[�t�F�[�X�ɕύX
//-------------------------------------------------------------------

#include "CubeUnit.h"
#include "StageCube_1.h"
#include "Engine.h"	

// �R���X�g���N�^
ACubeUnit::ACubeUnit()
	: mCubeMesh(NULL)
	, mRootComponent(NULL)
	, mIsSelected(false)
	, serialNum(0)
	, mXCoordinate(0)
	, mYCoordinate(0)
	, mZCoordinate(0)
	, canChangeMaterial(false)
	, mCubeMatInterface_1(NULL)
	, mCubeMatInterface_2(NULL)
	, mCubeMatInterface_3(NULL)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MyRootSceneComponent"));
	RootComponent = mRootComponent;

	mCubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mCubeMesh"));

	mCubeMesh->SetupAttachment(RootComponent);

	mCubeMatInterface_1 = CreateDefaultSubobject<UMaterialInterface>(TEXT("CubeMatInterface1"));
	mCubeMatInterface_2 = CreateDefaultSubobject<UMaterialInterface>(TEXT("CubeMatInterface2"));
	mCubeMatInterface_3 = CreateDefaultSubobject<UMaterialInterface>(TEXT("CubeMatInterface3"));

	// �}�E�X�J�[�\�����d�˂Ă��鎞
	mCubeMesh->OnBeginCursorOver.AddUniqueDynamic(this, &ACubeUnit::OnOver2);
	mCubeMesh->OnEndCursorOver.AddUniqueDynamic(this, &ACubeUnit::EndOver2);
	//OnBeginCursorOver.AddUniqueDynamic(this, &ACubeUnit::OnOver);
	//OnEndCursorOver.AddUniqueDynamic(this, &ACubeUnit::EndOver);

}

// ���s���Ɉ�x�Ă΂��
void ACubeUnit::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* myPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	 myPlayerController->bEnableMouseOverEvents = true;
	
	
	if (mCubeMesh != NULL)
	{
		// static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial(TEXT("/Game/Material/OutLine_V.OutLine_V'"));

		// mCubeMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

		if (mCubeMatInterface_1 != NULL)
		{
			mCubeMesh->SetMaterial(0, mCubeMatInterface_1);
		} // end if()
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("CubeMaterial_1 Loading Failed")));
		} // end else

	} // end if()
	
} // void BeginPlay()


// ���t���[���Ă΂��
void ACubeUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

} // void Tick()


// dont need  0903
void ACubeUnit::SetMeshAndMaterialOnBegin(UStaticMesh* newMesh, UMaterial* newMaterial_1, UMaterial* newMaterial_2, UMaterial* newMaterial_3)
{

	if (newMesh != NULL)
	{
		// mCubeMesh->SetStaticMesh(newMesh);

		if (newMaterial_1 != NULL && newMaterial_2 != NULL && newMaterial_3 != NULL)
		{
			//mCubeMaterial_1 = newMaterial_1;
			//mCubeMaterial_2 = newMaterial_2;
			//mCubeMaterial_3 = newMaterial_3;

			//mCubeMesh->SetMaterial(0, mCubeMaterial_1);
		} // end if()
		else GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("newMaterial is NULL!")));
		
		//else UE_LOG(LogTemp, Log, TEXT("newMaterial is NULL"));

	} // end if()
	else GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("newMesh is NULL!")));

	// else UE_LOG(LogTemp, Log, TEXT("newMesh is NULL"));

}  // void SetMeshAndMaterialOnBegin


void ACubeUnit::OnOver2(UPrimitiveComponent* Target)
{
	// �}�e���A���ύX�ł��Ȃ���ԂȂ�߂�
	if (canChangeMaterial == false)
		return;

	if (mCubeMesh != NULL)
	{
		if (mIsSelected == false)
		{
			if (mCubeMatInterface_3 != NULL)
			{
				mCubeMesh->SetMaterial(0, mCubeMatInterface_3);
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
	// �}�e���A���ύX�ł��Ȃ���ԂȂ�߂�
	if (canChangeMaterial == false)
		return;

	if (mCubeMesh != NULL)
	{
		if (mIsSelected == false)
		{
			if (mCubeMatInterface_1 != NULL)
			{
				mCubeMesh->SetMaterial(0, mCubeMatInterface_1);
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
			// ���I���̎�

			if (mCubeMatInterface_1 != NULL)
			{
				mCubeMesh->SetMaterial(0, mCubeMatInterface_1);
			} // end if()
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("CubeMatInterface_1 Fail Loading")));
			} // end else
		} // end if()
		else
		{
			// �I�����Ă���̎�

			if (mCubeMatInterface_2 != NULL)
			{
				mCubeMesh->SetMaterial(0, mCubeMatInterface_2);
			} // end if()
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("CubeMatInterface_2 Fail Loading")));
			} // end else
		} // end else
	} // end if()

} // void ChangeMaterialFunc()

void ACubeUnit::ChangeToDefaultMaterial()
{
	if (mCubeMesh != NULL)
	{
		// Test for Current Material
		// UE_LOG(LogTemp, Log, TEXT(" Fail %s"), *mCubeMesh->GetMaterial(0)->GetMaterial()->GetName());

		if (mCubeMesh->GetMaterial(0)->GetMaterial() != mCubeMatInterface_1) {

			if (mCubeMatInterface_1 != NULL)
			{
				mCubeMesh->SetMaterial(0, mCubeMatInterface_1);
			} // end if()
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("CubeMatInterface_1 Fail Loading")));
			} // end else
		} // end if()
	} // end if()

} // void ChangeToDefaultMaterial()


ACubeUnit::~ACubeUnit()
{
}

