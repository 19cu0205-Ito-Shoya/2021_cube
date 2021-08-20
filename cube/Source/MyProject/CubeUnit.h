//-------------------------------------------------------------------
// ファイル		：CubeUnit.h
// 概要			：単体Cubeの処理
// 作成者		：19CU0236 林雲暉 
// 作成日		：2021/08/11		単体Cubeの基本構成制作
// 更新日		：2021/08/12		選択機能実装、マテリアル変えれる
//				：2021/08/15		選択機能の回転用ガイドラインの処理追加
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
	// コンストラクタ
	ACubeUnit();
	// デストラクタ
	~ACubeUnit();

protected:
	// 実行時に一度呼ばれる
	virtual void BeginPlay() override;

public:	
	// 毎フレーム呼ばれる
	virtual void Tick(float DeltaTime) override;

public:
	// シーンコンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GuideLineDetails", meta = (AllowPrivateAccess = "true"))
		USceneComponent* mRootComponent;

	// 選択されましたか?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeDetails|Variables")
		bool mIsSelected;

	// Debug用 + Editorで見える位置番号
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeDetails|Variables")
		int serialNum;

	// 配列用の座標位置 X
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeDetails|Variables")
		int mXCoordinate;

	// 配列用の座標位置 Y
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeDetails|Variables")
		int mYCoordinate;

	// 配列用の座標位置 Z
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeDetails|Variables")
		int mZCoordinate;

	// マテリアルを変える
	UFUNCTION()
		void ChangeMaterialFunc();

private:
	// 本体のメッシュ
	UPROPERTY(EditAnywhere, Category = "CubeDetails|Mesh")
		UStaticMeshComponent* mCubeMesh;

	// Default Material デフォルトマテリアル
	UPROPERTY(EditAnywhere, Category = "CubeDetails|Mesh")
		UMaterial* mCubeMaterial_1;

	// Chosen Material 選択されたマテリアル
	UPROPERTY(EditAnywhere, Category = "CubeDetails|Mesh")
		UMaterial* mCubeMaterial_2;

	// Cursor Over's Material マウスが上にいる時のマテリアル
	UPROPERTY(EditAnywhere, Category = "GuideLineDetails|Mesh")
		UMaterial* mCubeMaterial_3;

	// 保留中マテリアル
	UPROPERTY(EditAnywhere, Category = "CubeDetails|Mesh")
		UMaterialInstanceDynamic* mCubeMaterial_Instance;



private:
	// UFUNCTION()
	//	void ChangeMaterialMode(int index, UMaterial* newMaterial);

	UFUNCTION()
		void OnSelected(AActor* Target, FKey ButtonPressed);





};


