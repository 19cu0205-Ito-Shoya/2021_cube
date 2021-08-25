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


	// �}�E�X�J�[�\�����d�˂Ă��鎞
	mCubeMesh->OnBeginCursorOver.AddUniqueDynamic(this, &ACubeUnit::OnOver2);
	mCubeMesh->OnEndCursorOver.AddUniqueDynamic(this, &ACubeUnit::EndOver2);
	//OnBeginCursorOver.AddUniqueDynamic(this, &ACubeUnit::OnOver);
	//OnEndCursorOver.AddUniqueDynamic(this, &ACubeUnit::EndOver);


	// Test Spawnning Diff Cube  21-08-25
	if (testInt == 1)
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("CubeUnit ConsTr")));


}

// ���s���Ɉ�x�Ă΂��
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


// ���t���[���Ă΂��
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

