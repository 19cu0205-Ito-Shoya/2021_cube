//---------------------------------------------------------------------------------
// ファイル		：StageCube_1.h
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
	// コンストラクタ
	AStageCube_1();

	// デストラクタ
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

	// 右回転90度
	void RoatateTheCubesRight90( const int mode);

	// 左回転90度(=右回転270度)
	void RoatateTheCubesLeft90( const int mode);

	// 回転180度
	void RoatateTheCubes180( const int mode);

	// マウスのX軸移動
	void MoveMouseX(const float _axisValue);

	// マウスのY軸移動
	void MoveMouseY(const float _axisValue);

	// マウスの左ボタンを押す
	void MouseLeftButtonPressed();

	// マウスの左ボタンを離す
	void MouseLeftButtonReleased();

	// Pitchが90度に越えないため、Rotatorを計算する関数
	FRotator MyCombineRotators(const FRotator _A, const FRotator _B) const
	{
		return FRotator( (_A.Quaternion()) * (_B.Quaternion()));
	}; // FRotator MyCombineRotators()


	void SwapCoordinate(ACubeUnit* _A, ACubeUnit* _B);

	// カメラ回転スケールを調整する
	float AdjustCameraTurnScale( const float _inPitch );

	// For Debug (Debug用 + Editorで見える)ため、回転後の場所を三つのArray再配置
	void Replace3Array();


	UFUNCTION()
		void OnSelected(AActor* Target, FKey ButtonPressed);

public:
	// 単体Cubeをガイドラインにアタッチ
	void AttachToGuideLine( const int mode );
	
	// 単体Cubeをガイドラインからデタッチ、総体Cubeにアタッチ
	void DetachFromGuideLine();

	// ガイドラインの位置を設定
	void SetGuideLinePosition();

	// ガイドラインの可視性を変更
	void ChangeAllGuideLinesVisibility(const bool isVisible);

	void DeSelectCubeAndGuide( bool deSelectCube, bool deSelectGuide);

	void SetSelectingGuideLine(const bool isSelect);

	void SetSelectingCube(const bool isSelect);

	// 回転の角度を矯正する
	void NormalizeGuideRotation();

	void ManageGuideLineRotateResultToArray();

private:
	// Rootシーンコンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USceneComponent* mRootComponent;

	// Cube's シーンコンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USceneComponent* mCubesRootComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeDetails", meta = (AllowPrivateAccess = "true"))
		float mMouseXvalue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeDetails", meta = (AllowPrivateAccess = "true"))
		float mMouseYvalue;

	// スプリングアーム
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* mSpringArm;

	// カメラ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		UCameraComponent* mCamera;

	// 単位CubeBPの生成用
	UPROPERTY(BlueprintReadOnly, Category = "CubeDetails", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class AActor> bp_CubeUnit;

	// 単位CubeBPのpath
	UPROPERTY(BlueprintReadOnly, Category = "CubeDetails", meta = (AllowPrivateAccess = "true"))
		FString path = "Blueprint'/Game/BP/BP_CubeUnit.BP_CubeUnit_C'";


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GuideLine", meta = (AllowPrivateAccess = "true"))
		float guideLineTurnningScale;

public:
	// Nested containersのためEditorとBlueprintには表示できない
	TArray< TArray< TArray<ACubeUnit*>>> CubeArray3D;						// 単位Cubeの集合

	// カメラ回転速度のスケール
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraDetails", meta = (AllowPrivateAccess = "true"))
		float mCameraTurnScaleX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraDetails", meta = (AllowPrivateAccess = "true"))
		float mCameraTurnScaleY;

	// 今はCubeを選択いていますか?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeDetails", meta = (AllowPrivateAccess = "true"))
		bool isSelectingCube;

	// 今選択しているCube
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeDetails", meta = (AllowPrivateAccess = "true"))
		ACubeUnit* mCurrentSelectedCube;

	// 今はガイドラインを選択いていますか?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GuideLine", meta = (AllowPrivateAccess = "true"))
		bool isSelectingGuideLine;

	// 今選択しているガイドライン
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GuideLine", meta = (AllowPrivateAccess = "true"))
		AGuideLineZ* mCurrentSelectedGuideLine;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeDetails", meta = (AllowPrivateAccess = "true"))
		bool isDraggingGuideLine;

	// 全体Cubeの回転フラグ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeDetails", meta = (AllowPrivateAccess = "true"))
		bool isMovingCamera;

	// 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeDetails", meta = (AllowPrivateAccess = "true"))
		FRotator mStartRotateDegree;

private:

	// X軸のガイドライン
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GuideLine", meta = (AllowPrivateAccess = "true"))
		AGuideLineZ* mGuideLineXaxis;

	// Y軸のガイドライン
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GuideLine", meta = (AllowPrivateAccess = "true"))
		AGuideLineZ* mGuideLineYaxis;

	// Z軸のガイドライン
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GuideLine", meta = (AllowPrivateAccess = "true"))
		AGuideLineZ* mGuideLineZaxis;


	// Editorでチェックするため (Debug用)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeDetails", meta = (AllowPrivateAccess = "true"))
		TArray<AActor*> d1Cube;

	// Editorでチェックするため (Debug用)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeDetails", meta = (AllowPrivateAccess = "true"))
		TArray<AActor*> d2Cube;

	// Editorでチェックするため (Debug用)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeDetails", meta = (AllowPrivateAccess = "true"))
		TArray<AActor*> d3Cube;




};
