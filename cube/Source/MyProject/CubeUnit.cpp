//-------------------------------------------------------------------
// �t�@�C��		�FCubeUnit.cpp
// �T�v			�F�P��Cube�̏���
// �쐬��		�F19CU0236 �щ_�� 
// �쐬��		�F2021/08/11		�P��Cube�̊�{�\������
// �X�V��		�F2021/08/12		�I���@�\�����A�}�e���A���ς����
//				�F2021/08/15		�I���@�\�̉�]�p�K�C�h���C���̏����ǉ�
//				�F2021/08/23		�I���@�\�̃C���v�b�g�C�x���g��StageCube�Ɉړ�����
//-------------------------------------------------------------------

#include "CubeUnit.h"
#include "StageCube_1.h"
#include "Engine.h"	

// �R���X�g���N�^
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

// ���s���Ɉ�x�Ă΂��
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


// ���t���[���Ă΂��
void ACubeUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

} // void Tick()


void ACubeUnit::OnSelected(AActor* Target, FKey ButtonPressed)
{

} // void OnSelected

void ACubeUnit::ChangeMaterialFunc()
{
	if (mCubeMesh != NULL)
	{
		if (mIsSelected == false) {
			// ���I���̎�

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
			// �I�����Ă���̎�

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

