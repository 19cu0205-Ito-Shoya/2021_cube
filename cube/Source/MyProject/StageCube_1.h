//---------------------------------------------------------------------------------
// �t�@�C��		�FStageCube_1.h
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

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "CubeUnit.h"
#include "GuideLineZ.h"
#include "StageCube_1.generated.h"

UCLASS()
class MYPROJECT_API AStageCube_1 : public APawn
{
	GENERATED_BODY()

public:
	// �R���X�g���N�^
	AStageCube_1();

	// �f�X�g���N�^
	~AStageCube_1();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


private:

	// �E��]90�x
	void RoatateTheCubesRight90( const int mode);

	// ����]90�x(=�E��]270�x)
	void RoatateTheCubesLeft90( const int mode);

	// ��]180�x
	void RoatateTheCubes180( const int mode);

	// �}�E�X��X���ړ�
	void MoveMouseX(const float _axisValue);

	// �}�E�X��Y���ړ�
	void MoveMouseY(const float _axisValue);

	// �}�E�X�̍��{�^��������
	void MouseLeftButtonPressed();

	// �}�E�X�̍��{�^���𗣂�
	void MouseLeftButtonReleased();

	// Pitch��90�x�ɉz���Ȃ����߁ARotator���v�Z����֐�
	FRotator MyCombineRotators(const FRotator _A, const FRotator _B) const
	{
		return FRotator( (_A.Quaternion()) * (_B.Quaternion()));
	}; // FRotator MyCombineRotators()


	void SwapCoordinate(ACubeUnit* _A, ACubeUnit* _B);

	// �J������]�X�P�[���𒲐�����
	float AdjustCameraTurnScale( const float _inPitch );

	// For Debug (Debug�p + Editor�Ō�����)���߁A��]��̏ꏊ���O��Array�Ĕz�u
	void Replace3Array();


	UFUNCTION()
		void OnSelected(AActor* Target, FKey ButtonPressed);

public:
	// �P��Cube���K�C�h���C���ɃA�^�b�`
	void AttachToGuideLine( const int mode );
	
	// �P��Cube���K�C�h���C������f�^�b�`�A����Cube�ɃA�^�b�`
	void DetachFromGuideLine();

	// �K�C�h���C���̈ʒu��ݒ�
	void SetGuideLinePosition();

	// �K�C�h���C���̉�����ύX
	void ChangeAllGuideLinesVisibility(const bool isVisible);

	void DeSelectCubeAndGuide( bool deSelectCube, bool deSelectGuide);

	void SetSelectingGuideLine(const bool isSelect);

	void SetSelectingCube(const bool isSelect);

	// ��]�̊p�x����������
	void NormalizeGuideRotation();

	void ManageGuideLineRotateResultToArray();

private:
	// Root�V�[���R���|�[�l���g
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USceneComponent* mRootComponent;

	// Cube's �V�[���R���|�[�l���g
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USceneComponent* mCubesRootComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeDetails", meta = (AllowPrivateAccess = "true"))
		float mMouseXvalue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeDetails", meta = (AllowPrivateAccess = "true"))
		float mMouseYvalue;

	// �X�v�����O�A�[��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* mSpringArm;

	// �J����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		UCameraComponent* mCamera;

	// �P��CubeBP�̐����p
	UPROPERTY(BlueprintReadOnly, Category = "CubeDetails", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class AActor> bp_CubeUnit;

	// �P��CubeBP��path
	UPROPERTY(BlueprintReadOnly, Category = "CubeDetails", meta = (AllowPrivateAccess = "true"))
		FString path = "Blueprint'/Game/BP/BP_CubeUnit.BP_CubeUnit_C'";


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GuideLine", meta = (AllowPrivateAccess = "true"))
		float guideLineTurnningScale;

public:
	// Nested containers�̂���Editor��Blueprint�ɂ͕\���ł��Ȃ�
	TArray< TArray< TArray<ACubeUnit*>>> CubeArray3D;						// �P��Cube�̏W��

	// �J������]���x�̃X�P�[��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraDetails", meta = (AllowPrivateAccess = "true"))
		float mCameraTurnScaleX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraDetails", meta = (AllowPrivateAccess = "true"))
		float mCameraTurnScaleY;

	// ����Cube��I�����Ă��܂���?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeDetails", meta = (AllowPrivateAccess = "true"))
		bool isSelectingCube;

	// ���I�����Ă���Cube
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeDetails", meta = (AllowPrivateAccess = "true"))
		ACubeUnit* mCurrentSelectedCube;

	// ���̓K�C�h���C����I�����Ă��܂���?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GuideLine", meta = (AllowPrivateAccess = "true"))
		bool isSelectingGuideLine;

	// ���I�����Ă���K�C�h���C��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GuideLine", meta = (AllowPrivateAccess = "true"))
		AGuideLineZ* mCurrentSelectedGuideLine;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeDetails", meta = (AllowPrivateAccess = "true"))
		bool isDraggingGuideLine;

	// �S��Cube�̉�]�t���O
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeDetails", meta = (AllowPrivateAccess = "true"))
		bool isMovingCamera;

	// 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeDetails", meta = (AllowPrivateAccess = "true"))
		FRotator mStartRotateDegree;

private:

	// X���̃K�C�h���C��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GuideLine", meta = (AllowPrivateAccess = "true"))
		AGuideLineZ* mGuideLineXaxis;

	// Y���̃K�C�h���C��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GuideLine", meta = (AllowPrivateAccess = "true"))
		AGuideLineZ* mGuideLineYaxis;

	// Z���̃K�C�h���C��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GuideLine", meta = (AllowPrivateAccess = "true"))
		AGuideLineZ* mGuideLineZaxis;


	// Editor�Ń`�F�b�N���邽�� (Debug�p)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeDetails", meta = (AllowPrivateAccess = "true"))
		TArray<AActor*> d1Cube;

	// Editor�Ń`�F�b�N���邽�� (Debug�p)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeDetails", meta = (AllowPrivateAccess = "true"))
		TArray<AActor*> d2Cube;

	// Editor�Ń`�F�b�N���邽�� (Debug�p)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeDetails", meta = (AllowPrivateAccess = "true"))
		TArray<AActor*> d3Cube;




};
