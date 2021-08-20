//-------------------------------------------------------------------
// �t�@�C��		�FGuideLineZ.h
// �T�v			�F��]�p�K�C�h���C���̏���
// �쐬��		�F19CU0236 �щ_�� 
// �쐬��		�F2021/08/12		��]�p�K�C�h���C���̊�{�\������
// �X�V��		�F2021/08/13		�I���@�\�����A�}�e���A���ς����
//				�F2021/08/14		���������ǉ��A�\����\���ǉ�
//-------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "GuideLineZ.generated.h"

UCLASS()
class MYPROJECT_API AGuideLineZ : public AActor
{
	GENERATED_BODY()
	
public:	
	// �R���X�g���N�^
	AGuideLineZ();

	// �f�X�g���N�^
	~AGuideLineZ();

protected:
	// ���s���Ɉ�x�Ă΂��
	virtual void BeginPlay() override;

public:	
	// ���t���[���Ă΂��
	virtual void Tick(float DeltaTime) override;


public:
	// �{�̂̃��b�V��
	UPROPERTY(EditAnywhere, Category = "GuideLineDetails|Mesh")
		UStaticMeshComponent* mGuideLineMesh;

	// �V�[���R���|�[�l���g
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GuideLineDetails", meta = (AllowPrivateAccess = "true"))
		USceneComponent* mRootComponent;

	// �I������܂�����?
	UPROPERTY(VisibleAnywhere, Category = "GuideLineDetails")
		bool mIsSelected;

	// �\������܂�����?
	UPROPERTY(VisibleAnywhere, Category = "GuideLineDetails")
		bool mIsVisible;

	//���͂ǂ����H
	// mode 0 = XY-Plane(Z-axis),  1 = YZ-Plane(X-axis),  2 = XZ-Plane(Y-axis)
	UPROPERTY(VisibleAnywhere, Category = "GuideLineDetails")
		int mode;

private:
	// Default Material �f�t�H���g�}�e���A��
	UPROPERTY(EditAnywhere, Category = "GuideLineDetails|Mesh")
		UMaterial* mLineMaterial_1;

	// Chosen Material �I�����ꂽ�}�e���A��
	UPROPERTY(EditAnywhere, Category = "GuideLineDetails|Mesh")
		UMaterial* mLineMaterial_2;

	// Cursor Over's Material �}�E�X����ɂ��鎞�̃}�e���A��
	UPROPERTY(EditAnywhere, Category = "GuideLineDetails|Mesh")
		UMaterial* mLineMaterial_3;


private:
	UFUNCTION()
		void OnSelected(AActor* Target, FKey ButtonPressed);

public:
	// �}�e���A����ς���
	UFUNCTION()
		void ChangeMaterialFunc();

	// ������ς���
	UFUNCTION()
		void ChangeVisibilityFunc();

};
