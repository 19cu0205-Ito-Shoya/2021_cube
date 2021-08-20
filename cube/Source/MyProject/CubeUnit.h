//-------------------------------------------------------------------
// �t�@�C��		�FCubeUnit.h
// �T�v			�F�P��Cube�̏���
// �쐬��		�F19CU0236 �щ_�� 
// �쐬��		�F2021/08/11		�P��Cube�̊�{�\������
// �X�V��		�F2021/08/12		�I���@�\�����A�}�e���A���ς����
//				�F2021/08/15		�I���@�\�̉�]�p�K�C�h���C���̏����ǉ�
//-------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "CubeUnit.generated.h"

UCLASS()
class MYPROJECT_API ACubeUnit : public AActor
{
	GENERATED_BODY()
	
public:	
	// �R���X�g���N�^
	ACubeUnit();
	// �f�X�g���N�^
	~ACubeUnit();

protected:
	// ���s���Ɉ�x�Ă΂��
	virtual void BeginPlay() override;

public:	
	// ���t���[���Ă΂��
	virtual void Tick(float DeltaTime) override;

public:
	// �V�[���R���|�[�l���g
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GuideLineDetails", meta = (AllowPrivateAccess = "true"))
		USceneComponent* mRootComponent;

	// �I������܂�����?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeDetails|Variables")
		bool mIsSelected;

	// Debug�p + Editor�Ō�����ʒu�ԍ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeDetails|Variables")
		int serialNum;

	// �z��p�̍��W�ʒu X
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeDetails|Variables")
		int mXCoordinate;

	// �z��p�̍��W�ʒu Y
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeDetails|Variables")
		int mYCoordinate;

	// �z��p�̍��W�ʒu Z
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeDetails|Variables")
		int mZCoordinate;

	// �}�e���A����ς���
	UFUNCTION()
		void ChangeMaterialFunc();

private:
	// �{�̂̃��b�V��
	UPROPERTY(EditAnywhere, Category = "CubeDetails|Mesh")
		UStaticMeshComponent* mCubeMesh;

	// Default Material �f�t�H���g�}�e���A��
	UPROPERTY(EditAnywhere, Category = "CubeDetails|Mesh")
		UMaterial* mCubeMaterial_1;

	// Chosen Material �I�����ꂽ�}�e���A��
	UPROPERTY(EditAnywhere, Category = "CubeDetails|Mesh")
		UMaterial* mCubeMaterial_2;

	// Cursor Over's Material �}�E�X����ɂ��鎞�̃}�e���A��
	UPROPERTY(EditAnywhere, Category = "GuideLineDetails|Mesh")
		UMaterial* mCubeMaterial_3;

	// �ۗ����}�e���A��
	UPROPERTY(EditAnywhere, Category = "CubeDetails|Mesh")
		UMaterialInstanceDynamic* mCubeMaterial_Instance;



private:
	// UFUNCTION()
	//	void ChangeMaterialMode(int index, UMaterial* newMaterial);

	UFUNCTION()
		void OnSelected(AActor* Target, FKey ButtonPressed);





};


