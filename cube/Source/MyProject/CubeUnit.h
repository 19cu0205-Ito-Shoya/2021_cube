//-------------------------------------------------------------------
// ファイル		：CubeUnit.h
// 概要			：単体Cubeの処理
// 作成者		：19CU0236 林雲暉 
// 作成日		：2021/08/11		単体Cubeの基本構成制作
// 更新日		：2021/08/12		選択機能実装、マテリアル変えれる
//				：2021/08/15		選択機能の回転用ガイドラインの処理追加
//				：2021/08/23		選択機能のインプットイベントをStageCubeに移動した
//				：2021/08/25		マウスカーソル重ねる時のマテリアル変更
//				：2021/08/26		マテリアル変更できるか判断追加
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

	// 配列用の座標位置 Z
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeDetails|Variables")
		int mZCoordinate;

	// 配列用の座標位置 X
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeDetails|Variables")
		int mXCoordinate;

	// 配列用の座標位置 Y
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeDetails|Variables")
		int mYCoordinate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeDetails|Variables")
		bool canChangeMaterial;


	// マテリアルを変える
	UFUNCTION()
		void ChangeMaterialFunc();

	UFUNCTION()
		void ChangeToDefaultMaterial();

public:
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
	UPROPERTY(EditAnywhere, Category = "CubeDetails|Mesh")
		UMaterial* mCubeMaterial_3;

	// 保留中マテリアル
	UPROPERTY(EditAnywhere, Category = "CubeDetails|Mesh")
		UMaterialInstanceDynamic* mCubeMaterial_Instance;

public:
	UFUNCTION()
		void SetMeshAndMaterialOnBegin(UStaticMesh* newMesh, UMaterial* newMaterial_1, UMaterial* newMaterial_2, UMaterial* newMaterial_3);

	UFUNCTION()
		void Test123(int x);

	UFUNCTION()
		void TestSet456(int y);

private:

	int testInt;

	// UFUNCTION()
	//	void ChangeMaterialMode(int index, UMaterial* newMaterial);

	UFUNCTION()
		void OnSelected(AActor* Target, FKey ButtonPressed);



public:

	// マウスカーソルが重ねている時
	UFUNCTION()
		void OnOver2(UPrimitiveComponent* Target);

	// マウスカーソルがこのCubeから離れた時
	UFUNCTION()
		void EndOver2(UPrimitiveComponent* Target);


};


