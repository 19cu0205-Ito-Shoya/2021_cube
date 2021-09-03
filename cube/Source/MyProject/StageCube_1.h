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
//				�F2021/08/22		�I����@�̏C��
//				�F2021/08/23		�}�E�X�̃C���v�b�g�C�x���g��StageCube�ɂ܂Ƃ߂�
//				�F2021/08/24		�}�E�X�̃��C���g���[�X���@��ύX
//				�F2021/08/25		�K�C�h���C����]�̎���I���̃K�C�h���C�����\��
//				�F2021/08/26		�K�C�h���C���̉�]�����́A�}�E�X�J�[�\���̈ʒu�ɂ���ĉ�]����
//				�F2021/08/27		�P��Cube���}�e���A���ύX�ł��邩��ǉ��A�ǂ�Collision�ǉ�
//				�F2021/09/02		���ꂼ���Cube�̃��b�V���ƃ}�e���A����ݒ肵�Đ������邱�Ƃ�ǉ�
//				�F2021/09/03		�}�e���A�����}�e���A���C���^�[�t�F�[�X�ɕύX�A�K�C�h���C���̃f�^�b�`���s�������̌�����ǉ�
//---------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"							// TEnumAsByte<EDrawDebugTrace>���`���邽��
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

	// Cube�̔z����W�ʒu������
	void SwapCoordinate(ACubeUnit* _A, ACubeUnit* _B);

	// �J������]�X�P�[���𒲐�����
	float AdjustCameraTurnScale( const float _inPitch );

	// For Debug (Debug�p + Editor�Ō�����)���߁A��]��̏ꏊ���O��Array�Ĕz�u
	void Replace3Array();

	// �}�E�X�J�[�\������̃��C���g���[�X
	bool MouseLineTrace(FHitResult& MouseHitResult);

public:
	// �P��Cube���K�C�h���C���ɃA�^�b�`
	void AttachToGuideLine( const int mode );
	
	// �P��Cube���K�C�h���C������f�^�b�`�A����Cube�ɃA�^�b�`
	void DetachFromGuideLine();

	// �K�C�h���C���̈ʒu��ݒ�
	void SetGuideLinePosition();

	// �K�C�h���C���̉�����ύX
	void ChangeAllGuideLinesVisibility(const bool isVisible);

	// �K�C�h���C�����h���b�O���鎞�A�I�����Ă��Ȃ��K�C�h���C���̂��\��
	void ChangeUnSelecetedGuideLineVisibility();

	// ���I�����Ă���K�C�h���C������ݒ�
	void SetSelectingGuideLine(const bool isSelect);

	// ���I�����Ă���Cube��ݒ�
	void SetSelectingCube(const bool isSelect);

	// ��]�̊p�x����������
	void NormalizeGuideRotation();

	// �K�C�h���C���̉�]������ݒ�
	void DecideGuideLineTurnningDirection();

	// �I�����ĂȂ��P��Cube���}�e���A����ύX�ł��邩��ݒ�
	void SetUnselectCubeUnitsCanChangeMat(bool canCgange);

	// �ی��p - �K�C�h���C���̃f�^�b�`���s�������A���������B
	void CheckAllGuideLinesGetDetached();

	// �P��Cube�� num(�ԍ�) �̃��b�V�����擾
	UStaticMesh* GetSpecificCubeMesh(int num);

public:
	// Cube�ƃK�C�h���C���̑I��������
	UFUNCTION(BlueprintCallable)
		void DeSelectCubeAndGuide(bool deSelectCube, bool deSelectGuide);

	// �S���̒P��Cube���}�e���A����ύX�ł��邩��ݒ�
	UFUNCTION(BlueprintCallable)
		void SetAllCubeUnitsCanChangeMat(bool canCgange);



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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GuideLine", meta = (AllowPrivateAccess = "true"))
		float guideLineTurnningScaleX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GuideLine", meta = (AllowPrivateAccess = "true"))
		float guideLineTurnningScaleY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CubeDetails", meta = (AllowPrivateAccess = "true"))
		float mouseTraceDistance;

	// Debug Line�̕\�����@
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CubeDetails", meta = (AllowPrivateAccess = "true"))
		TEnumAsByte<EDrawDebugTrace::Type> mDrawDebugType;

	// �O�̕ǂ�Collision
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeDetails", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* mFrontWallCollision;

	// ��̕ǂ�Collision
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeDetails", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* mBackWallCollision;

	// ���̕ǂ�Collision
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeDetails", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* mLeftWallCollision;

	// �E�̕ǂ�Collision
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeDetails", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* mRightWallCollision;

public:
	// Nested containers�̂���Editor��Blueprint�ɂ͕\���ł��Ȃ�
	TArray< TArray< TArray<ACubeUnit*>>> CubeArray3D;						// �P��Cube�̏W��

	// �J������]���x�̃X�P�[��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraDetails", meta = (AllowPrivateAccess = "true"))
		float mCameraTurnScaleX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraDetails", meta = (AllowPrivateAccess = "true"))
		float mCameraTurnScaleY;

	// need to change to EditDefaultsOnly
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CubeDetails", meta = (AllowPrivateAccess = "true"))
		float mCubeDistance;

	// need to change to EditDefaultsOnly
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CubeDetails", meta = (AllowPrivateAccess = "true"))
		FVector mBoxCollisionSize;

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

	// �K�C�h���C�����h���b�O���Ă��邩
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeDetails", meta = (AllowPrivateAccess = "true"))
		bool isDraggingGuideLine;

	// �K�C�h���C�����h���b�O�n�߂̈ʒu
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeDetails", meta = (AllowPrivateAccess = "true"))
		FVector2D mStartDraggingPosition;

	// �S��Cube�̉�]�t���O
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeDetails", meta = (AllowPrivateAccess = "true"))
		bool isMovingCamera;

	// �J�[�\���̈ړ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CubeDetails", meta = (AllowPrivateAccess = "true"))
		float minimumCursorsDisplacement;

	// �K�C�h���C����]�J�n�̊p�x
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeDetails", meta = (AllowPrivateAccess = "true"))
		FRotator mStartRotateDegree;


private:
	// �P��Cube�̃X�P�[��
	UPROPERTY(EditDefaultsOnly, Category = "CubeUnitSettings", meta = (AllowPrivateAccess = "true"))
		FVector mCubeUnitScale;

	// �P��Cube�A���ꂼ��̃��b�V��
	UPROPERTY(EditDefaultsOnly, Category = "CubeUnitSettings", meta = (AllowPrivateAccess = "true"))
		UStaticMesh* mCubeMesh1;
	UPROPERTY(EditDefaultsOnly, Category = "CubeUnitSettings", meta = (AllowPrivateAccess = "true"))
		UStaticMesh* mCubeMesh2;
	UPROPERTY(EditDefaultsOnly, Category = "CubeUnitSettings", meta = (AllowPrivateAccess = "true"))
		UStaticMesh* mCubeMesh3;
	UPROPERTY(EditDefaultsOnly, Category = "CubeUnitSettings", meta = (AllowPrivateAccess = "true"))
		UStaticMesh* mCubeMesh4;
	UPROPERTY(EditDefaultsOnly, Category = "CubeUnitSettings", meta = (AllowPrivateAccess = "true"))
		UStaticMesh* mCubeMesh5;
	UPROPERTY(EditDefaultsOnly, Category = "CubeUnitSettings", meta = (AllowPrivateAccess = "true"))
		UStaticMesh* mCubeMesh6;
	UPROPERTY(EditDefaultsOnly, Category = "CubeUnitSettings", meta = (AllowPrivateAccess = "true"))
		UStaticMesh* mCubeMesh7;
	UPROPERTY(EditDefaultsOnly, Category = "CubeUnitSettings", meta = (AllowPrivateAccess = "true"))
		UStaticMesh* mCubeMesh8;
	UPROPERTY(EditDefaultsOnly, Category = "CubeUnitSettings", meta = (AllowPrivateAccess = "true"))
		UStaticMesh* mCubeMesh9;
	UPROPERTY(EditDefaultsOnly, Category = "CubeUnitSettings", meta = (AllowPrivateAccess = "true"))
		UStaticMesh* mCubeMesh10;
	UPROPERTY(EditDefaultsOnly, Category = "CubeUnitSettings", meta = (AllowPrivateAccess = "true"))
		UStaticMesh* mCubeMesh11;
	UPROPERTY(EditDefaultsOnly, Category = "CubeUnitSettings", meta = (AllowPrivateAccess = "true"))
		UStaticMesh* mCubeMesh12;
	UPROPERTY(EditDefaultsOnly, Category = "CubeUnitSettings", meta = (AllowPrivateAccess = "true"))
		UStaticMesh* mCubeMesh13;
	UPROPERTY(EditDefaultsOnly, Category = "CubeUnitSettings", meta = (AllowPrivateAccess = "true"))
		UStaticMesh* mCubeMesh14;
	UPROPERTY(EditDefaultsOnly, Category = "CubeUnitSettings", meta = (AllowPrivateAccess = "true"))
		UStaticMesh* mCubeMesh15;
	UPROPERTY(EditDefaultsOnly, Category = "CubeUnitSettings", meta = (AllowPrivateAccess = "true"))
		UStaticMesh* mCubeMesh16;
	UPROPERTY(EditDefaultsOnly, Category = "CubeUnitSettings", meta = (AllowPrivateAccess = "true"))
		UStaticMesh* mCubeMesh17;
	UPROPERTY(EditDefaultsOnly, Category = "CubeUnitSettings", meta = (AllowPrivateAccess = "true"))
		UStaticMesh* mCubeMesh18;
	UPROPERTY(EditDefaultsOnly, Category = "CubeUnitSettings", meta = (AllowPrivateAccess = "true"))
		UStaticMesh* mCubeMesh19;
	UPROPERTY(EditDefaultsOnly, Category = "CubeUnitSettings", meta = (AllowPrivateAccess = "true"))
		UStaticMesh* mCubeMesh20;
	UPROPERTY(EditDefaultsOnly, Category = "CubeUnitSettings", meta = (AllowPrivateAccess = "true"))
		UStaticMesh* mCubeMesh21;
	UPROPERTY(EditDefaultsOnly, Category = "CubeUnitSettings", meta = (AllowPrivateAccess = "true"))
		UStaticMesh* mCubeMesh22;
	UPROPERTY(EditDefaultsOnly, Category = "CubeUnitSettings", meta = (AllowPrivateAccess = "true"))
		UStaticMesh* mCubeMesh23;
	UPROPERTY(EditDefaultsOnly, Category = "CubeUnitSettings", meta = (AllowPrivateAccess = "true"))
		UStaticMesh* mCubeMesh24;
	UPROPERTY(EditDefaultsOnly, Category = "CubeUnitSettings", meta = (AllowPrivateAccess = "true"))
		UStaticMesh* mCubeMesh25;
	UPROPERTY(EditDefaultsOnly, Category = "CubeUnitSettings", meta = (AllowPrivateAccess = "true"))
		UStaticMesh* mCubeMesh26;
	UPROPERTY(EditDefaultsOnly, Category = "CubeUnitSettings", meta = (AllowPrivateAccess = "true"))
		UStaticMesh* mCubeMesh27;


	// Default Material �f�t�H���g�}�e���A�� (MaterialInterface)
	UPROPERTY(EditDefaultsOnly, Category = "CubeUnitSettings")
		UMaterialInterface* mCubeMatInterface_1;

	// Chosen Material �I�����ꂽ�}�e���A��
	UPROPERTY(EditDefaultsOnly, Category = "CubeUnitSettings")
		UMaterialInterface* mCubeMatInterface_2;

	// Cursor Over's Material �}�E�X����ɂ��鎞�̃}�e���A��
	UPROPERTY(EditDefaultsOnly, Category = "CubeUnitSettings")
		UMaterialInterface* mCubeMatInterface_3;


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
